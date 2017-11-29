# 1.在pom.xml里修改
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/1.jpg)
# 2.右键项目
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/2.jpg)
# 3.重新加载maven包，即可导入selenium
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/3.jpg)
# 4.最新的chromedriver与chrome的对应关系表<br>
chromedriver版本	支持的Chrome版本<br>
v2.33	v60-62<br>
v2.32	v59-61<br>
v2.31	v58-60<br>
v2.30	v58-60<br>
v2.29	v56-58<br>
v2.28	v55-57<br>
v2.27	v54-56<br>
v2.26	v53-55<br>
v2.25	v53-55<br>
v2.24	v52-54<br>
v2.23	v51-53<br>
v2.22	v49-52<br>
v2.21	v46-50<br>
v2.20	v43-48<br>
v2.19	v43-47<br>
v2.18	v43-46<br>
v2.17	v42-43<br>
v2.13	v42-45<br>
v2.15	v40-43<br>
v2.14	v39-42<br>
v2.13	v38-41<br>
v2.12	v36-40<br>
v2.11	v36-40<br>
v2.10	v33-36<br>
v2.9	v31-34<br>
v2.8	v30-33<br>
v2.7	v30-33<br>
v2.6	v29-32<br>
v2.5	v29-32<br>
v2.4	v29-32<br>
附：<br> 
所有chromedriver均可在下面链接中下载到：<br>
http://chromedriver.storage.googleapis.com/index.html<br>
# 5.出现bug，按如下修改后，重新运行第二步
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/4.png)
You have a version conflict in Guava. Selenium WebDriver (not WebDriverManager) 
depends transitively of a given version of Guava, and it seems you are using another 
one in your project. I would use the latest versions of both.<br>
修改如下部分：<br>
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/5.png)
# 6.新建test.java,内容如下：<br>
```Java
/**
 * Created by tangwei on 2017/11/26.
 */
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;
import org.junit.Test;
/**
 * Created by deng on 2017/5/15.
 */
public class SeleniumTest {

    @Test
    public  void test() throws InterruptedException {
        System.setProperty("webdriver.chrome.driver","/usr/local/bin/chromedriver"); // 此处PATH替换为你的chromedriver所在路径
        WebDriver webDriver = new ChromeDriver();
        webDriver.get("http://www.baidu.com/");
//        System.out.println("The testing page title is: " + webDriver.getTitle());
//
//        webDriver.quit();
    }
}
```
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/6.png)
# 7.后续测试参考网址：http://www.denizoguz.com/2015/06/25/using-selenium-chrome-driver-on-mac-os-x/
