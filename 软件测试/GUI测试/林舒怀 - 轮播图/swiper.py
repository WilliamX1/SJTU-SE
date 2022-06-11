# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.
from selenium import webdriver
from time import sleep


def test_swiper():
    """
    应用场景：轮播图
    """
    print('\n通过find_element_by_class_name方法进行测试...')
    try:
        driver = webdriver.Chrome('/Users/linshuhuai/Downloads/chromedriver')
        driver.get('https://www.sjtu.edu.cn')
        sleep(5)

        pictures = set()
        for i in range(40):
            pictures.add(
                driver.find_element_by_class_name("owl-item.animated.owl-animated-in.fadeIn.active").find_element_by_tag_name('img').get_attribute(
                    'src'))
            print('等待轮播图播放...(%ds)' % (40 - i), end='\r')
            sleep(1)
        assert pictures.__len__() == 6
        paths = ['https://www.sjtu.edu.cn/resource/upload/202205/20220506_152254_952.jpg',
                 'https://www.sjtu.edu.cn/resource/upload/202205/20220507_152013_453.jpg',
                 'https://www.sjtu.edu.cn/resource/upload/202205/20220504_150418_418.jpg',
                 'https://www.sjtu.edu.cn/resource/upload/202204/20220430_192343_848.jpg',
                 'https://www.sjtu.edu.cn/resource/upload/202204/20220425_175549_905.jpg',
                 'https://www.sjtu.edu.cn/resource/upload/202203/20220330_154905_202.jpg']
        for path in paths:
            assert path in pictures
        print('轮播图正常自动播放完毕')

        for i in range(1, 7):
            driver.find_element_by_xpath('//*[@id="main"]/div[1]/div/div[1]/div[3]/div[%d]' % i).click()
            sleep(0.5)

            assert 'owl-item animated owl-animated-in fadeIn active' in driver.find_element_by_xpath(
                '//*[@id="main"]/div[1]/div/div[1]/div[1]/div/div[%d]' % (i+3)).get_attribute('class')
        print('测试点击控制轮播图完毕')

    finally:
        sleep(3)
        driver.quit()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    test_swiper()
# See PyCharm help at https://www.jetbrains.com/help/pycharm/
