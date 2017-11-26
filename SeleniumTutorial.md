1.在pom.xml里修改
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/1.jpg)
2.右键项目
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/2.jpg)
3.重新加载maven包，即可导入selenium
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/3.jpg)
4.最新的chromedriver与chrome的对应关系表<br>
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
5.出现bug
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/4.png)
You have a version conflict in Guava. Selenium WebDriver (not WebDriverManager) 
depends transitively of a given version of Guava, and it seems you are using another 
one in your project. I would use the latest versions of both.<br>
修改如下部分：<br>
![Alt text](https://github.com/942860759/DocumentCollection/blob/master/img/5.png)
