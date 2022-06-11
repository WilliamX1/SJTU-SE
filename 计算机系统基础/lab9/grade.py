#!/usr/bin/env python2

import pexpect
import random
import signal
import sys
import re
import binascii
import string
import traceback
import time
import datetime
import argparse
import subprocess
import os

DEFAULT_TIMEOUT=3
outside_proxy = None
outside_server = None
verbose = True
strace = False
quiet = False

possible_ip = map(lambda s:s[5:], subprocess.check_output("ip addr | grep -o -E 'inet 192[^/]*|inet 127[^/]*'", shell=True).split("\n")[:-1])

class LabErrorException(Exception):
    def __init__(self, hint):
        self.hint = hint
    def __str__(self):
        return hint

def limit(s, start=0):
    r = ""
    if start < 0:
        start = 0
    if start > 0:
        r += "...(%d bytes) " % (start)
    r += binascii.hexlify(s[start:start + 10])
    if start + 10 < len(s):
        r += " ..."
    return r

def limited_binary_diff(a, b):
    if a == None and b == None:
        return "(null)", "(null)"
    elif a == None:
        return "(null)", limit(b)
    elif b == None:
        return limit(a), "(null)"
    else:
        for i in xrange(min(len(a), len(b))):
            if a[i] != b[i]:
                return limit(a, i - 5), limit(b, i - 5)
        return limit(a), limit(b)

class MismatchException(LabErrorException):
    def __init__(self, hint, expect, got, binary=False):
        self.hint = hint
        if binary:
            self.expect, self.got = limited_binary_diff(expect, got)
        else:
            if expect == None:
                self.expect = ""
            else:
                self.expect = expect.strip()
            if got == None:
                self.got = ""
            else:
                self.got = got.strip()
    def __str__(self):
        return self.hint + "\n" + \
               ("\tExpect: %s\n" % (self.expect)) + \
               ("\tGot:    %s" % (self.got))

class ProxyDieException(LabErrorException):
    def __init__(self, hint="Your proxy died"):
        self.hint = hint
    def __str__(self):
        return self.hint

class MissingLogException(LabErrorException):
    def __init__(self, client_addr, uri, size, part_output):
        self.client_addr = client_addr
        self.uri = uri
        self.part_output = part_output
        self.size = size
    def __str__(self):
        return "Your proxy didn't output log in following format:\n" + \
                "\t%s: %s %s %d\n" % (datetime.datetime.utcnow().strftime("%a %d %b %Y %H:%M:%S UTC"), self.client_addr, self.uri, self.size) + \
                "Part of your output is:\n" + \
                "\t%s" % (self.part_output)

def start_server():
    if strace:
        server = pexpect.spawn("bash -c 'stty -echo -icanon; strace -f -tt ./server 2>>server-strace.log'", timeout=DEFAULT_TIMEOUT)
    else:
        server = pexpect.spawn("bash -c 'stty -echo -icanon; ./server'", timeout=DEFAULT_TIMEOUT)
    server.expect("ready\r\n")
    if strace:
        server.real_pid = int(subprocess.check_output("pgrep -P $(pgrep -P %d)" % (server.pid), shell=True))
    else:
        server.real_pid = int(subprocess.check_output("pgrep -P %d" % (server.pid), shell=True))
    port = int(server.readline())
    server.port = port
    server.sendline("accept")
    return (port, server)

def start_proxy(listen_port):
    if strace:
        proxy = pexpect.spawn("bash -c 'strace -f -tt ./proxy %d 2>>proxy-strace.log'" % (listen_port), timeout=DEFAULT_TIMEOUT, logfile=open("proxy.log", "a"))
    else:
        proxy = pexpect.spawn("bash -c './proxy %d 2>&1'" % (listen_port), timeout=DEFAULT_TIMEOUT, logfile=open("proxy.log", "a"))
    proxy.listen_port = listen_port
    time.sleep(0.3)
    return proxy

def start_client(port):
    ip = random.choice(possible_ip)
    if strace:
        proc = pexpect.spawn("bash -c 'stty -echo -icanon; strace -f -tt ./client %s %d 2>>client-strace.log'" % (ip, port), timeout=DEFAULT_TIMEOUT)
    else:
        proc = pexpect.spawn("bash -c 'stty -echo -icanon; ./client %s %d'" % (ip, port), timeout=DEFAULT_TIMEOUT)
    proc.expect("ready\r\n", timeout=1)
    proc.ip = ip
    try:
        proc.expect("connected\r\n", timeout=1)
    except pexpect.exceptions.EOF:
        raise ProxyDieException("Can't connect to your proxy")
    except pexpect.exceptions.TIMEOUT:
        raise ProxyDieException("Can't connect to your proxy")
    if strace:
        proc.real_pid = int(subprocess.check_output("pgrep -P $(pgrep -P %d)" % (proc.pid), shell=True))
    else:
        proc.real_pid = int(subprocess.check_output("pgrep -P %d" % (proc.pid), shell=True))
    return proc

def restart_proxy(proxy):
    if proxy != None:
        listen_port = proxy.listen_port
        proxy.close()
        proxy.wait()
        return start_proxy(listen_port)
    else:
        return None

def restart_server(server):
    helper_close(server)
    helper_accept(server)

def restart_client(client):
    helper_close(client)
    helper_reconnect(client)

def restart_sc(server, client):
    restart_server(server)
    restart_client(client)

def restart_env(server, client, proxy):
    proxy = restart_proxy(proxy)
    helper_close(server)
    helper_close(client)
    helper_accept(server)
    helper_reconnect(client)
    return proxy

def check_proxy_alive(proxy):
    if proxy != None:
        if not proxy.isalive():
            raise ProxyDieException()

def helper_send(proc, s):
    proc.sendline("send %d" % (len(s)))
    proc.sendline(binascii.hexlify(s))

def helper_eof(proc):
    proc.sendline("shutdown")

def helper_close(proc):
    proc.sendline("close")
    os.kill(proc.real_pid, signal.SIGUSR1)
    proc.expect("closed\r\n")

def helper_reconnect(proc):
    proc.sendline("reconnect")
    try:
        proc.expect("connected\r\n")
    except pexpect.exceptions.TIMEOUT:
        raise ProxyDieException("Can't connect to your proxy")
    except pexpect.exceptions.EOF:
        raise ProxyDieException("Can't connect to your proxy")

def helper_accept(proc):
    proc.sendline("accept")

def proxy_uri(server_ip, server_port, path):
    return "http://%s:%d%s" % (server_ip, server_port, path)

def proxy_request(method, server_ip, server_port, path):
    return "%s %s HTTP/1.1\r\n" % (method, proxy_uri(server_ip, server_port, path))

def helper_readline(proc, timeout=-1):
    proc.sendline("readline")
    resp = proc.readline()
    if resp == "error\r\n":
        return None
    else:
        try:
            return binascii.unhexlify(resp.strip())
        except:
            print repr(resp)
            raise

def helper_read(proc, n, timeout=-1):
    proc.sendline("read %d" % (n))
    resp = proc.readline()
    if resp == "error\r\n":
        return None
    else:
        return binascii.unhexlify(resp.strip())

def random_bytes(n):
    return "".join([chr(random.randint(0, 255)) for i in xrange(n)])

def random_string(n):
    return "".join([random.choice(string.letters + string.digits) for i in xrange(n)])

def generate_headers(candidates):
    headers = []
    for i in candidates:
        if random.random() < 0.6:
            headers += [(i, random.choice(candidates[i]))]
    return headers

def expect_data(proc, data, timeout=-1):
    if proc != None:
        if data != None:
            recv_data = helper_read(proc, len(data), timeout=timeout)
            if data != recv_data:
                raise MismatchException(
                        "Someone got wrong HTTP body",
                        data, recv_data, True)

# For request
req_header_candidates = {
    "Accept": ["text/plain", "application/octet-stream", "image/jpeg", "video/mpeg"],
    "Accept-Charset": ["utf-8", "gb2312", "latin-1"],
    "Host": ["www.baidu.com", "www.google.com", "ipads.se.sjtu.edu.cn", "www.cnblogs.com"],
    "Authorization": ["Basic YWRtaW46YWRtaW4=", "Basic YWRta67cYWRtaW4="],
    "Connection": ["close", "keep-alive"]
}

class HttpContent(object):
    def __init__(self, head_line, headers, body):
        self.head_line = head_line
        self.headers = headers
        self.body = body

        self.raw = head_line
        self.head_line_end = len(self.raw)
        for header in headers:
            self.raw += "%s: %s\r\n" % (header[0], header[1])
        self.header_end = len(self.raw)
        self.raw += "\r\n"
        self.header_end_end = len(self.raw)
        if body != None:
            self.raw += body
        self.body_end = len(self.raw)

class HttpRequest(HttpContent):
    def __init__(self, method, server_ip, server_port, path, body=None):
        self.method = method
        self.server_ip = server_ip
        self.server_port = server_port
        self.path = path

        headers = generate_headers(req_header_candidates)
        if body != None:
            headers += [("Content-Length", str(len(body)))]

        HttpContent.__init__(self, proxy_request(method, server_ip, server_port, path), headers, body)

def send_req(proc, req):
    helper_send(proc, req.raw)

def send_req_crash(proc, req, crash_point):
    helper_send(proc, req.raw[:crash_point])

def expect_req(proc, req, timeout=-1):
    if proc != None:
        line = helper_readline(proc)
        if line != "%s %s HTTP/1.1\r\n" % (req.method, req.path):
            raise MismatchException("Server got wrong request line", "%s %s HTTP/1.1" % (req.method, req.path), line)
        for header in req.headers:
            line = helper_readline(proc)
            if line != "%s: %s\r\n" % (header[0], header[1]):
                raise MismatchException("Server got wrong header line", "%s: %s" % (header[0], header[1]), line)
        line = helper_readline(proc)
        if line != "\r\n":
            raise MismatchException("Server got wrong request end line", "", line)
        if req.body != None:
            recv_body = helper_read(proc, len(req.body), timeout=timeout)
            if req.body != recv_body:
                raise MismatchException(
                        "Server got wrong HTTP body",
                        req.body, recv_body, True)

# For response
resp_header_candidates = {
    "Server": [],
    "Accept-Charset": ["utf-8", "gb2312", "latin-1"],
    "Host": ["www.baidu.com", "www.google.com", "ipads.se.sjtu.edu.cn", "www.cnblogs.com"],
    "Authorization": ["Basic YWRtaW46YWRtaW4=", "Basic YWRta67cYWRtaW4="],
    "Connection": ["close", "keep-alive"]
}
for i in xrange(10):
    resp_header_candidates["Server"] += [random_string(random.randint(10, 20))]

status_desc = {
    200: "OK",
    400: "Bad Request",
    401: "Unauthorized",
    403: "Forbidden",
    404: "Not Found",
    500: "Internal Server Error"
}
def random_status():
    return random.choice(status_desc.keys())

class HttpResponse(HttpContent):
    def __init__(self, status, body=None):
        self.status = status

        headers = generate_headers(resp_header_candidates)
        if body != None:
            headers += [("Content-Length", str(len(body)))]

        HttpContent.__init__(self, "HTTP/1.1 %d %s\r\n" % (status, status_desc[status]), headers, body)

def send_resp(proc, resp):
    helper_send(proc, resp.raw)

def send_resp_crash(proc, resp, crash_point):
    helper_send(proc, resp.raw[:crash_point])

def expect_resp_header_only(proc, resp, timeout=-1):
    if proc != None:
        line = helper_readline(proc)
        if line != resp.head_line:
            #raise MismatchException("Client got wrong response line", "%s %s HTTP/1.1" % (req.method, req.path), line)
            raise MismatchException("Client got wrong response line", "HTTP/1.1 %d %s\r\n"  % (resp.status, status_desc[resp.status]), line)
        for header in resp.headers:
            line = helper_readline(proc)
            if line != "%s: %s\r\n" % (header[0], header[1]):
                raise MismatchException("Client got wrong header line", "%s: %s" % (header[0], header[1]), line)
        line = helper_readline(proc)
        if line != "\r\n":
            raise MismatchException("Client got wrong response header end line", "", line)

def expect_resp(proc, resp, timeout=-1):
    if proc != None:
        expect_resp_header_only(proc, resp, timeout)
        if resp.body != None:
            recv_body = helper_read(proc, len(resp.body), timeout=timeout)
            if resp.body != recv_body:
                raise MismatchException(
                        "Client got wrong HTTP body",
                        resp.body, recv_body, True)

def build_env():
    server = None
    client = None
    proxy = None
    try:
        if outside_server == None:
            server_port, server = start_server()
        else:
            server_port = outside_server
        if outside_proxy == None:
            proxy_port = random.randint(1024, 65535)
            while proxy_port == server_port:
                proxy_port = random.randint(1024, 65535)
            proxy = start_proxy(proxy_port)
        else:
            proxy_port = outside_proxy
        client = start_client(proxy_port)
        return (server, server_port, client, proxy)
    except:
        if server != None:
            server.kill(signal.SIGKILL)
        if client != None:
            client.kill(signal.SIGKILL)
        if proxy != None:
            proxy.kill(signal.SIGKILL)
        raise

def build_sc(proxy):
    server = None
    client = None
    try:
        if outside_server == None:
            server_port, server = start_server()
        else:
            raise Exception("Can't test concurrency when using outside server")
        client = start_client(proxy.listen_port)
        return (server, server_port, client)
    except:
        if server != None:
            server.kill(signal.SIGKILL)
        if client != None:
            client.kill(signal.SIGKILL)
        raise

def general_test(fn, score, desc, quiet):
    if not quiet:
        print "[%2d pts] %s..." % (score, desc),
    sys.stdout.flush()
    server = None
    client = None
    proxy = None
    try:
        server, server_port, client, proxy = build_env()
        fn(server, server_port, client, proxy)
        if not quiet:
            print "Pass"
        return True
    except pexpect.exceptions.TIMEOUT, e:
        if not quiet:
            print "Timeout"
        if verbose:
            traceback.print_exc()
        return False
    except LabErrorException, e:
        if not quiet:
            print "Fail"
        if verbose:
            traceback.print_exc()
        elif not quiet:
            print e
        return False
    finally:
        if server != None:
            server.kill(signal.SIGKILL)
        if client != None:
            client.kill(signal.SIGKILL)
        if proxy != None:
            proxy.kill(signal.SIGKILL)

def send_body(proc, body):
    helper_send(proc, body)

def log_regex(client_addr, uri, size):
    return re.compile("... [0-9]+ ... [0-9]+ [0-9][0-9]:[0-9][0-9]:[0-9][0-9] ...: " + re.escape("%s %s %d\r\n" % (client_addr, uri, size)), re.M)

def expect_log(proc, client_addr, uri, size):
    if proc != None:
        try:
            proc.expect(log_regex(client_addr, uri, size))
        except pexpect.exceptions.TIMEOUT:
            raise MissingLogException(client_addr, uri, size, proc.buffer)

def do_part1_1(server, server_port, client, proxy):
    server_ip = random.choice(possible_ip)
    req = HttpRequest("GET", server_ip, server_port, "/")
    send_req(client, req)
    expect_req(server, req)
    resp = HttpResponse(random_status(), random_bytes(random.randint(300, 500)))
    send_resp(server, resp)
    expect_resp(client, resp)
    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, "/"), len(resp.raw))

# Check GET without parameter
# Pts: 2
def do_part1_2(server, server_port, client, proxy):
    server_ip = random.choice(possible_ip)
    path = "/"
    path += random_string(random.randint(10, 20))
    path += "/"
    path += random_string(random.randint(10, 20))
    path += "?"
    path += random_string(random.randint(10, 20))
    path += "="
    path += random_string(random.randint(10, 20))
    path += "&"
    path += random_string(random.randint(10, 20))
    path += "="
    path += random_string(random.randint(10, 20))
    req = HttpRequest("GET", server_ip, server_port, path)
    send_req(client, req)
    expect_req(server, req)
    resp = HttpResponse(random_status(), random_bytes(random.randint(300, 500)))
    send_resp(server, resp)
    expect_resp(client, resp)
    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, path), len(resp.raw))

def do_part1_3(server, server_port, client, proxy):
    # generate param
    server_ip = random.choice(possible_ip)
    path = "/"
    path += random_string(random.randint(10, 20))
    path += "/"
    path += random_string(random.randint(10, 20))
    body = ""
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_bytes(random.randint(300, 500))

    req = HttpRequest("POST", server_ip, server_port, path, body)
    send_req(client, req)
    expect_req(server, req)

    resp = HttpResponse(random_status(), random_bytes(random.randint(300, 500)))
    send_resp(server, resp)
    expect_resp(client, resp)
    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, path), len(resp.raw))

def do_part1_4(server, server_port, client, proxy, in_part1_7=False):
    # Generate param
    server_ip = random.choice(possible_ip)
    path = "/"
    path += random_string(random.randint(10, 20))
    path += "/"
    path += random_string(random.randint(10, 20))
    body = ""
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_bytes(random.randint(300, 500))
    req = HttpRequest("POST", server_ip, server_port, path, body)

    # crash in request
    send_req_crash(client, req, random.randint(1, req.head_line_end - 1))
    check_proxy_alive(proxy)

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    # crash in header
    send_req_crash(client, req, random.randint(req.head_line_end, req.header_end - 1))
    check_proxy_alive(proxy)

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    # crash in body
    send_req_crash(client, req, random.randint(req.header_end, req.body_end - 1))
    check_proxy_alive(proxy)

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    resp = HttpResponse(random_status(), random_bytes(random.randint(10000, 13000)))

    # crash in response
    send_req(client, req)
    expect_req(server, req)
    send_resp_crash(server, resp, random.randint(1, resp.head_line_end - 1))
    check_proxy_alive(proxy)

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    # crash in response header
    send_req(client, req)
    expect_req(server, req)
    send_resp_crash(server, resp, random.randint(resp.head_line_end, resp.header_end - 1))
    check_proxy_alive(proxy)

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    # crash in response body
    send_req(client, req)
    expect_req(server, req)
    send_resp_crash(server, resp, random.randint(resp.header_end, resp.body_end - 1))
    check_proxy_alive(proxy)

def do_part1_5(server, server_port, client, proxy):
    server_ip = random.choice(possible_ip)
    path = "/"
    path += random_string(random.randint(10, 20))
    path += "/"
    path += random_string(random.randint(10, 20))
    body = ""
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_string(random.randint(10, 20))
    body += "="
    body += random_string(random.randint(10, 20))
    body += "\r\n"
    body += random_bytes(random.randint(30, 50))

    req = HttpRequest("POST", server_ip, server_port, path, body)
    # Hack the send_req
    lag_point = [random.randint(1, len(req.raw) - 1) for i in xrange(5)]
    lag_point.sort()
    helper_send(client, req.raw[:lag_point[0]])
    time.sleep(1)
    for i in xrange(len(lag_point) - 1):
        helper_send(client, req.raw[lag_point[i]:lag_point[i + 1]])
        time.sleep(1)
    helper_send(client, req.raw[lag_point[-1]:])
    expect_req(server, req)

    resp = HttpResponse(random_status(), random_bytes(random.randint(300, 500)))
    lag_point = [random.randint(1, len(resp.raw) - 1) for i in xrange(5)]
    lag_point.sort()
    helper_send(server, resp.raw[:lag_point[0]])
    time.sleep(1)
    for i in xrange(len(lag_point) - 1):
        helper_send(server, resp.raw[lag_point[i]:lag_point[i + 1]])
        time.sleep(1)
    helper_send(server, resp.raw[lag_point[-1]:])
    expect_resp(client, resp)

    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, path), len(resp.raw))

def do_part1_6(server, server_port, client, proxy, in_part1_7=False):
    # Generate param
    server_ip = random.choice(possible_ip)
    long_path = "/" + random_string(random.randint(2000, 4000))
    long_path += "/"
    long_path += random_string(random.randint(2000, 3500))
    long_body = random_string(random.randint(10000, 13000))
    long_binary_body = random_bytes(random.randint(10000, 13000))
    long_resp = random_bytes(random.randint(10000, 13000))

    req = HttpRequest("POST", server_ip, server_port, long_path, long_body)
    send_req(client, req)
    expect_req(server, req)
    resp = HttpResponse(random_status(), long_resp)
    send_resp(server, resp)
    expect_resp(client, resp)
    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, long_path), len(resp.raw))

    if not in_part1_7:
        proxy = restart_env(server, client, proxy)
    else:
        restart_sc(server, client)

    server_ip = random.choice(possible_ip)
    req = HttpRequest("POST", server_ip, server_port, long_path, long_binary_body)
    send_req(client, req)
    expect_req(server, req)
    resp = HttpResponse(random_status(), long_resp)
    send_resp(server, resp)
    expect_resp(client, resp)
    expect_log(proxy, client.ip, proxy_uri(server_ip, server_port, long_path), len(resp.raw))

def do_part1_7(server, server_port, client, proxy):
    for i in xrange(3):
        if not quiet:
            print "Round %d..." % (i + 1),
            sys.stdout.flush()
        do_part1_1(server, server_port, client, proxy)
        restart_sc(server, client)
        do_part1_2(server, server_port, client, proxy)
        restart_sc(server, client)
        do_part1_3(server, server_port, client, proxy)
        restart_sc(server, client)
        do_part1_4(server, server_port, client, proxy, True)
        restart_sc(server, client)
        do_part1_5(server, server_port, client, proxy)
        restart_sc(server, client)
        do_part1_6(server, server_port, client, proxy, True)
        if i != 2:
            restart_sc(server, client)

PART2_1_ROUND = 1
PART2_1_CUT = 20
def do_part2_1(server, server_port, client, proxy):
    (server2, server_port2, client2) = build_sc(proxy)
    for i in xrange(PART2_1_ROUND):
        if not quiet:
            print "Round %d..." % (i + 1),
            sys.stdout.flush()
        bigfile = random_bytes(random.randint(1024 * 1024, 2 * 1024 * 1024))
        server_ip = random.choice(possible_ip)
        bigreq = HttpRequest("GET", server_ip, server_port, "/")
        send_req(client, bigreq)
        expect_req(server, bigreq)
        bigresp = HttpResponse(200, bigfile)
        cut_point = [random.randint(bigresp.header_end_end + 1, len(bigresp.raw) - 1) for i in xrange(PART2_1_CUT * 2 - 1)]
        cut_point += [len(bigresp.raw)]
        cut_point.sort()
        helper_send(server, bigresp.raw[:bigresp.header_end_end])
        expect_resp_header_only(client, bigresp)

        expect_logs = []
        expect_logs += [{
            "re": log_regex(client.ip, proxy_uri(server_ip, server_port, bigreq.path), len(bigresp.raw)),
            "client_addr": client.ip,
            "uri": proxy_uri(server_ip, server_port, bigreq.path),
            "size": len(bigresp.raw)
        }]
        for j in xrange(len(cut_point) / 2):
            if j == 0:
                helper_send(server, bigresp.raw[bigresp.header_end_end:cut_point[0]])
                expect_data(client, bigresp.raw[bigresp.header_end_end:cut_point[0]])
            else:
                helper_send(server, bigresp.raw[cut_point[j * 2 - 1]:cut_point[j * 2]])
                expect_data(client, bigresp.raw[cut_point[j * 2 - 1]:cut_point[j * 2]])
            server_ip = random.choice(possible_ip)
            smallreq = HttpRequest("POST", server_ip, server_port2, "/small", random_bytes(random.randint(1000, 1300)))
            send_req(client2, smallreq)
            expect_req(server2, smallreq)

            helper_send(server, bigresp.raw[cut_point[j * 2]:cut_point[j * 2 + 1]])
            expect_data(client, bigresp.raw[cut_point[j * 2]:cut_point[j * 2 + 1]])

            smallresp = HttpResponse(200, random_bytes(random.randint(1000, 3000)))
            send_resp(server2, smallresp)
            expect_resp(client2, smallresp)

            expect_logs += [{
                "re": log_regex(client2.ip, proxy_uri(server_ip, server_port2, smallreq.path), len(smallresp.raw)),
                "client_addr": client2.ip,
                "uri": proxy_uri(server_ip, server_port, smallreq.path),
                "size": len(smallresp.raw)
            }]
            restart_sc(server2, client2)

        restart_sc(server, client)

        logs = ""
        while True:
            try:
                logs += proxy.readline()
            except pexpect.exceptions.TIMEOUT:
                break
        for log in expect_logs:
            if log["re"].search(logs) == None:
                raise MissingLogException(log["client_addr"], log["uri"], log["size"], logs)

STEP = (100, 150)
class ConcurrentPayload(object):
    def __init__(self, server, proxy, req, resp):
        self.proxy = proxy
        self.client = start_client(proxy.listen_port)
        self.server = server
        self.req = req
        self.req_pos = 0
        self.req_done = False
        self.resp = resp
        self.resp_pos = 0
        self.resp_done = False
    def do(self):
        work_bytes = random.randint(*STEP)
        if not self.req_done:
            if work_bytes >= len(self.req.raw) - self.req_pos:
                work_bytes = len(self.req.raw) - self.req_pos
                self.req_done = True
            helper_send(self.client, self.req.raw[self.req_pos:self.req_pos + work_bytes])
            if work_bytes == len(self.req.raw) - self.req_pos:
                expect_req(self.server, self.req)
            self.req_pos += work_bytes
        if not self.resp_done:
            if work_bytes >= len(self.resp.raw) - self.resp_pos:
                work_bytes = len(self.resp.raw) - self.resp_pos
                self.resp_done = True
            helper_send(self.server, self.resp.raw[self.resp_pos:self.resp_pos + work_bytes])
            if work_bytes == len(self.resp.raw) - self.resp_pos:
                expect_resp(self.client, self.resp)
            self.resp_pos += work_bytes
        return self.req_done and self.resp_done
    def __str__(self):
        return "?"

CONCURRENCY = 10
def do_part2_2(server, server_port, client, proxy):
    servers = [(server_port, server)]
    for i in xrange(1, CONCURRENCY):
        servers += [start_server()]

    client_server_mapping = [i for i in xrange(CONCURRENCY)]
    random.shuffle(client_server_mapping)
    payload = []
    expect_logs = []
    for i in xrange(CONCURRENCY):
        server_ip = random.choice(possible_ip)
        payload += [ConcurrentPayload(servers[client_server_mapping[i]][1],
                                      proxy,
                                      HttpRequest("GET", server_ip, servers[client_server_mapping[i]][0], "/" + random_string(random.randint(10, 20))),
                                      HttpResponse(200, random_bytes(random.randint(100, 200))))]
        expect_logs += [{
            "re": log_regex(payload[-1].client.ip, proxy_uri(server_ip, payload[-1].server.port, payload[-1].req.path), len(payload[-1].resp.raw)),
            "client_addr": "127.0.0.1",
            "uri": proxy_uri(server_ip, payload[-1].server.port, payload[-1].req.path),
            "size": len(payload[-1].resp.raw)
        }]

    done = 0
    while done != CONCURRENCY:
        i = random.randint(0, len(payload) - 1)
        todo = payload[i]
        if todo.do():
            payload = payload[:i] + payload[i+1:]
            done += 1
    logs = ""
    while True:
        try:
            logs += proxy.readline()
        except pexpect.exceptions.TIMEOUT:
            break
    for log in expect_logs:
        if log["re"].search(logs) == None:
            raise MissingLogException(log["client_addr"], log["uri"], log["size"], logs)

test_part1_1 = {
    "score": 2,
    "desc": "Part I.1: GET without parameter",
    "fn": do_part1_1
}

test_part1_2 = {
    "score": 3,
    "desc": "Part I.2: GET with paramter",
    "fn": do_part1_2
}

test_part1_3 = {
    "score": 5,
    "desc": "Part I.3: POST",
    "fn": do_part1_3
}

test_part1_4 = {
    "score": 5,
    "desc": "Part I.4: server/client crash",
    "fn": do_part1_4
}

test_part1_5 = {
    "score": 5,
    "desc": "Part I.5: lag",
    "fn": do_part1_5
}

test_part1_6 = {
    "score": 5,
    "desc": "Part I.6: long line",
    "fn": do_part1_6
}

test_part1_7 = {
    "score": 10,
    "desc": "Part I.7: combine them all",
    "fn": do_part1_7
}

test_part2_1 = {
    "score": 15,
    "desc": "Part II.1: download big file + access web",
    "fn": do_part2_1
}

test_part2_2 = {
    "score": 15,
    "desc": "Part II.2: multiple small access",
    "fn": do_part2_2
}

candidate_tests = {
    "part1_1": [test_part1_1],
    "part1_2": [test_part1_2],
    "part1_3": [test_part1_3],
    "part1_4": [test_part1_4],
    "part1_5": [test_part1_5],
    "part1_6": [test_part1_6],
    "part1_7": [test_part1_7],
    "part1": [test_part1_1, test_part1_2, test_part1_3, test_part1_4, test_part1_5, test_part1_6, test_part1_7],
    "part2_1": [test_part2_1],
    "part2_2": [test_part2_2],
    "part2": [test_part2_1, test_part2_2],
    "all": [test_part1_1, test_part1_2, test_part1_3, test_part1_4, test_part1_5, test_part1_6, test_part1_7, test_part2_1, test_part2_2],
}

def main():
    global verbose
    global outside_proxy
    global quiet
    global strace
    parser = argparse.ArgumentParser(description="Test your lab 10.")
    parser.add_argument("--only", type=str, nargs=1, help="select one of tests")
    parser.add_argument("--outside", type=int, nargs=1, help="give a port so you can connect a proxy running outside")
    parser.add_argument("-v", action="store_true", dest="verbose", help="verbose mode")
    parser.add_argument("-t", action="store_true", dest="ta", help="TA mode, ignore all other args and only print final score")
    parser.add_argument("--strace", action="store_true", help="enable strace")
    args = parser.parse_args()
    if args.ta:
        args.only = ["all"]
        args.outside = None
        args.verbose = False
        args.strace = False
        quiet = True
    strace = args.strace
    verbose = args.verbose
    if args.outside != None:
        outside_proxy = args.outside[0]
    else:
        outside_proxy = args.outside
        
    if args.only == None:
        args.only = ["all"]

    tests = candidate_tests[args.only[0]]
    total_score = reduce(lambda s, t: s + t["score"], tests, 0)
    your_score = 0
    for test in tests:
        if general_test(test["fn"], test["score"], test["desc"], args.ta):
            your_score += test["score"]

    if args.ta:
        print your_score
    else:
        print
        print "Score: %d / %d" % (your_score, total_score)

if __name__ == "__main__":
    main()
