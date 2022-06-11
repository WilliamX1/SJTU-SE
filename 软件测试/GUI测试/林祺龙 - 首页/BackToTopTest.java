import io.github.bonigarcia.wdm.WebDriverManager;
import org.junit.jupiter.api.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.firefox.FirefoxDriver;

import static org.junit.jupiter.api.Assertions.*;

public class BackToTopTest {
    private static void waitSeconds() {
        try {
            Thread.sleep(2000);
        } catch (Exception ignored) {

        }
    }

    @Test
    public void test() {
        String url = "https://www.sjtu.edu.cn/";
        WebDriverManager.firefoxdriver().setup();
        WebDriver driver = new FirefoxDriver();
        driver.get(url);
        JavascriptExecutor executor = (JavascriptExecutor) driver;

        executor.executeScript("window.scrollTo(0,document.body.scrollHeight)");
        waitSeconds();

        WebElement toTop = driver.findElement(By.className("go-top"));
        toTop.click();
        waitSeconds();

        Long value = (Long) executor.executeScript("return window.pageYOffset;");
        assertEquals(0, value);

        driver.quit();
    }
}
