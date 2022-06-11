 #!/usr/bin/env python -W ignore::DeprecationWarning
from selenium import webdriver
from time import sleep
from selenium.webdriver.common.keys import Keys
import os



def init(fo):
    driverExe = '/Users/kangyixiao/EchoFile/2022/软件测试/hw/hw3/chromedriver'
    website_url = 'https://i.sjtu.edu.cn/xtgl/login_slogin.html'
    driver = webdriver.Chrome(driverExe)
    driver.get(website_url)
    sleep(3)
    assert driver.find_element_by_xpath('/html/head/title').get_attribute('textContent') == '上海交通大学教学信息服务网'
    
    fo.write('\t- current page title: {}\n'.format(driver.find_element_by_xpath('/html/head/title').get_attribute('textContent')))
    return driver

def test_login_text(driver,fo):
  text = driver.find_element_by_xpath('//*[@id="yhlogin"]').get_attribute('textContent')
  fo.write('\t==>通过 find_element_by_xpath 方法获取文字,验证为:'+text)
  assert(text == '登录')
  fo.write('\t==>成功获取文字内容：'+text+'\n')


def get_input_box(driver, fo):
    '''
    通过 find_element_by_id 等方法，获取出入境管理与服务中心的输入框
    '''
    fo.write("\n开始测试输入框（用户名和密码）\n")
    fo.write('\t==>通过 find_element_by_id/css/xpath 方法获取输入框...')

    inputBox_id = driver.find_element_by_id('yhm')
    inputBox_css = driver.find_element_by_css_selector('#yhm')
    inputBox_xpath = driver.find_element_by_xpath('//*[@id="yhm"]')

    assert inputBox_id == inputBox_css
    assert inputBox_id == inputBox_xpath
    # fo.write(inputBox_id.get_attribute('placeholder'))
    assert inputBox_id.get_attribute('placeholder') == '用户名'




    pwdBox_id = driver.find_element_by_id('mm')
    pwdBox_css = driver.find_element_by_css_selector('#mm')
    pwdBox_xpath=  driver.find_element_by_xpath('//*[@id="mm"]')

    assert pwdBox_id == pwdBox_css
    assert pwdBox_id == pwdBox_xpath
    fo.write( pwdBox_id.get_attribute("placeholder"))
    assert pwdBox_id.get_attribute("placeholder") == '密码'

    fo.write('\t==>成功获取用户名和密码输入框！\n')
    user = 'testUser'
    pwd = 'password'
  
    inputBox_id.send_keys(user)
    inputBox_id.send_keys(Keys.ENTER)
    pwdBox_id.send_keys(pwd)
    pwdBox_id.send_keys(Keys.ENTER)
    sleep(3)
    return inputBox_id

def test_login_button(driver, fo):
    fo.write('\n开始测试登陆按钮\n')
    fo.write('\t==>通过 find_element_by_xpath/id 方法获取登陆按钮\n')

    searchButton_xpath = driver.find_element_by_xpath('//*[@id="dl"]')
    searchButton_id = driver.find_element_by_id('dl')

    assert searchButton_xpath == searchButton_id
    assert searchButton_id.get_attribute('textContent')=='登 录' 

    fo.write('\t==>成功获取登录按钮\n')
    searchButton_id.click()
    fo.write(' - page title after click: {}'.format(driver.find_element_by_xpath('/html/head/title').get_attribute('textContent')))
    assert driver.find_element_by_xpath('/html/head/title').get_attribute('textContent') == '上海交通大学教学信息服务网'
    fo.write('\t==>按钮成功跳转至'+driver.find_element_by_xpath('/html/head/title').get_attribute('textContent')+'\n')
    fo.write('\t==>按钮文字为'+searchButton_id.get_attribute('textContent')+'\n')
    return searchButton_xpath



def test_href(driver, fo):
    '''
    通过 find_element_by_class_name 等方法获取跳转的链接\n'
    '''
    fo.write('\n开始测试链接\n')
    fo.write('\t==>通过 find_element_by_id/xpath 方法获取跳转的链接')

    routeLink_id = driver.find_element_by_id('authJwglxtLoginURL')
    routeLink_class_xpath = driver.find_element_by_xpath('//*[@id="authJwglxtLoginURL"]')
    
    assert routeLink_id == routeLink_class_xpath
    # print(routeLink_id.get_attribute("href"))
    assert routeLink_id.get_attribute("href") == 'https://i.sjtu.edu.cn/jaccountlogin'

    fo.write('\n==>成功获取跳转链接')
    fo.write('\n==>跳转链接名为'+routeLink_id.get_attribute("href"))
    routeLink_id.click()
    sleep(3) 
    fo.write(' - page title after click: {}'.format(driver.find_element_by_xpath('/html/head/title').get_attribute('textContent')))
    assert driver.find_element_by_xpath('/html/head/title').get_attribute('textContent') == '上海交通大学统一身份认证'
    fo.write('\n==>链接成功跳转: 上海交通大学统一身份认证\n')

    return routeLink_id






def test_login():
    '''
    应用场景：需要办理出入境业务的同学，通过搜索获得相应的文件并下载
    '''
    fo = open("result.txt", "w")
    fo.write('上海交通大学教学信息服务网 登陆功能测试开始...\n')
    # 获取 driver
    driver = init(fo)
    test_login_text(driver,fo)
    get_input_box(driver,fo)
    test_login_button(driver,fo)
    test_href(driver,fo)
    fo.write('\n所有测试全部通过！\n')
    fo.close()
    

if __name__ == '__main__':
    test_login()
    