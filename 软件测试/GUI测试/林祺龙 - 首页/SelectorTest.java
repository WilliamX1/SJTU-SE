import io.github.bonigarcia.wdm.WebDriverManager;
import org.junit.jupiter.api.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.firefox.FirefoxDriver;

import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;

public class SelectorTest {

    private static void waitSeconds() {
        try {
            Thread.sleep(1000);
        } catch (Exception ignored) {

        }
    }

    @Test
    public void test() {
        String url = "https://www.sjtu.edu.cn/";
        WebDriverManager.firefoxdriver().setup();
        WebDriver driver = new FirefoxDriver();
        driver.get(url);

        WebElement about = driver.findElement(By.linkText("关于交大"));
        about.click();

        WebElement blink = driver.findElement(By.linkText("学校简介"));
        blink.click();

        String currentWindow = driver.getWindowHandle();
        Set<String> handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        String newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/xxjj/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        about.click();
        blink = driver.findElement(By.linkText("历史沿革"));
        blink.click();
        handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/lsyg/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        about.click();
        blink = driver.findElement(By.linkText("交大领导"));
        blink.click();
        handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/jdld/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        about.click();
        blink = driver.findElement(By.linkText("历任领导"));
        blink.click();
        handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/lrld/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        about.click();
        blink = driver.findElement(By.linkText("交大年报"));
        blink.click();
        handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/jdnb/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        about.click();
        blink = driver.findElement(By.linkText("数据统计"));
        blink.click();
        handles = driver.getWindowHandles();
        handles.remove(currentWindow);
        newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);

        waitSeconds();
        assertEquals("https://www.sjtu.edu.cn/sjtj/index.html", driver.getCurrentUrl());
        driver.close();
        driver.switchTo().window(currentWindow);

        driver.quit();
    }
}
