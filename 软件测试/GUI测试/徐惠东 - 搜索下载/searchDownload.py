from selenium import webdriver
from time import sleep
from selenium.webdriver.common.keys import Keys
import os


def init():
    driverExe = '/Users/huid/Desktop/software-testing/GUI-Test/chromedriver'
    website_url = 'http://cgcj.sjtu.edu.cn/Material'
    driver = webdriver.Chrome(driverExe)
    driver.get(website_url)
    sleep(3)
    assert driver.find_element_by_xpath('/html/head/title').get_attribute('textContent') == '上海交通大学出入境管理与服务中心'
    
    print('\t- current page title: {}\n'.format(driver.find_element_by_xpath('/html/head/title').get_attribute('textContent')))
    return driver

def get_input_box(driver):
    '''
    通过 find_element_by_id 等方法，获取出入境管理与服务中心的输入框
    '''
    print('\t==>通过 find_element_by_id/tag/css/xpath 方法获取输入框...')

    inputBox_id = driver.find_element_by_id('Search')
    inputBox_tag = driver.find_element_by_tag_name('input')
    inputBox_css = driver.find_element_by_css_selector('#Search')
    inputBox_xpath = driver.find_element_by_xpath('//*[@id="Search"]')

    assert inputBox_id == inputBox_tag
    assert inputBox_id == inputBox_css
    assert inputBox_id == inputBox_xpath

    print('\t==>成功获取输入框！\n')
    return inputBox_id


def get_search_button(driver):
    '''
    通过 find_element_by_class_name 等方法，获取出入境管理与服务中心的搜索按钮
    '''
    print('\t==>通过 find_element_by_xpath 方法获取搜索按钮\n')

    searchButton_xpath = driver.find_element_by_xpath('//*[@id="vueControl"]/div/div[2]/div[2]/div[1]/i')
    searchButton_class_name = driver.find_element_by_class_name('icon-search')

    assert searchButton_xpath == searchButton_class_name

    print('\t==>成功获取搜索按钮\n')
    return searchButton_xpath


def get_href(driver):
    '''
    通过 find_element_by_class_name 等方法获取跳转的链接\n'
    '''
    print('\t==>通过 find_element_by_class_name 方法获取跳转的链接')

    routeLink_class_name = driver.find_element_by_class_name('item')
    routeLink_class_xpath = driver.find_element_by_xpath('//*[@id="vueControl"]/div/div[2]/div[2]/div[2]/ul/li/a')
    
    assert routeLink_class_name == routeLink_class_xpath

    print('\t==>成功获取跳转链接')
    return routeLink_class_name


def get_download(driver):
    '''
    通过 find_elements_by_xpath 获取子元素
    '''
    print('\t==>通过 find_elements_by_xpath 获取子元素')

    links_xpath = []
    links_xpath.append(driver.find_element_by_xpath('//*[@id="vueControl"]/div/div[2]/div[2]/div[2]/div[2]/div[4]/a[1]'))
    links_xpath.append(driver.find_element_by_xpath('//*[@id="vueControl"]/div/div[2]/div[2]/div[2]/div[2]/div[4]/a[2]'))

    print('\t==>成功获取下载链接')
    return links_xpath


def download_and_verify(driver, links):
    '''
    依次点击下载链接，等待下载结束后，验证本地文件系统中是否有相应的文件
    '''
    download_filepath = '/Users/huid/Downloads/'
    for link in links:
        link.click()
        sleep(3) # 等待下载结束
        download_files = os.listdir(download_filepath) # 获取 Download 文件夹中所有文件
        flag = False
        for filename in download_files:
            if filename[0:5] == link.text[0:5]:
                flag = True
        assert flag == True
    
    print('\t==>成功下载并验证文件')
    return True



def test_search_and_download():
    '''
    应用场景：需要办理出入境业务的同学，通过搜索获得相应的文件并下载
    '''
    print('通过文本搜索查询并下载文件测试开始...\n')
    # 获取 driver
    driver = init()
    # 获取输入框
    inputBod = get_input_box(driver)
    # 获取搜索按钮
    searchButton = get_search_button(driver)
    # 向输入框填入文字并点击搜索按钮
    inputBod.send_keys('承诺函')
    searchButton.click()
    sleep(1)
    # 切换到跳转后的窗口
    windows = driver.window_handles
    driver.switch_to.window(windows[-1])
    # 获取跳转链接
    routeLink = get_href(driver)
    # 点击跳转
    routeLink.click()
    # 切换到跳转后的窗口
    windows = driver.window_handles
    driver.switch_to.window(windows[-1])
    # 获取下载链接
    links = get_download(driver)
    # 点击下载链接并验证
    assert download_and_verify(driver, links) == True

    print("文本搜索查询并下载文件测试通过！\n")


if __name__ == '__main__':
    test_search_and_download()
    print('\n所有测试全部通过！\n')