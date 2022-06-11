import io.github.bonigarcia.wdm.WebDriverManager;
import org.junit.jupiter.api.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.firefox.FirefoxDriver;

import java.io.File;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;

public class ViewPdfAndDownloadTest {
    private static void waitSeconds() {
        try {
            Thread.sleep(10000);
        } catch (Exception ignored) {

        }
    }

    @Test
    public void test() {
        String url = "https://www.sjtu.edu.cn/sjtj/index.html";
        WebDriverManager.firefoxdriver().setup();
        WebDriver driver = new FirefoxDriver();
        driver.get(url);

        WebElement li = driver.findElement(By.className("col-sm-4"));
        li.click();

        String currentWindow = driver.getWindowHandle();
        Set<String> handles = driver.getWindowHandles();
        handles.remove(currentWindow);

        String newWindow = handles.iterator().next();
        driver.switchTo().window(newWindow);
        String[] path = driver.getTitle().split("/");
        String filename = path[path.length - 1];

        WebElement download = driver.findElement(By.id("download"));
        download.click();

        waitSeconds();

        File file = new File("C:\\Users\\51642\\Downloads\\" + filename);

        assertTrue(file.exists());
        if (!file.delete()) {
            System.out.println("Fail to delete.");
        }

        driver.quit();
    }
}
