
----------

#### 使用Scrapy爬取doubanTop250 ####

5/15/2019 5 :33 :30 PM 

Author:terrycoder

----------

##### 安装部分

##### 使用步骤

1. 创建工程
>scrapy startproject "爬虫工程名称"

2. 添加爬虫类
>scrapy genspider "爬虫类名"

3. 编写爬取数据Items.py

```
#序号
serial_number = scrapy.Field()
# 电影名
movie_name = scrapy.Field()
# 电影介绍
introduce = scrapy.Field()
# 星级
star = scrapy.Field()
# 电影评论数
evaluate = scrapy.Field()
# 电影描述
describe = scrapy.Field()
```
4.添加User-Agent
settings.py


```
USER_AGENT = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36'
```

5. 执行类打印

在parse()方法中添加response打印，然后执行
> scrapy crawl 爬虫类名

6. 添加main.py 方便pycharm执行

```
from scrapy import cmdline

cmdline.execute('scrapy crawl douban_spider'.split())
```
  
7. 添加url入口

```
# 入口url,扔到调度器里面去
 start_urls = ['https://movie.douban.com/top250']
```

8.编写parse()解析标签 

```
movie_list = response.xpath("//div[@class='article']/ol[@class='grid_view']/li")
    for i_item in movie_list:
        douban_item = SpiderItem()
        douban_item["serial_number"] = i_item.xpath(".//div[@class='pic']/em/text()").extract_first()
        douban_item["movie_name"] = i_item.xpath(".//div[@class='info']//div[@class='hd']/a/span[1]/text()").extract_first()
        douban_item["describe"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/p[@class='quote']/span/text()").extract_first()
        contents = i_item.xpath(".//div[@class='info']//div[@class='bd']/p[1]/text()").extract()

        for content in contents:
            content_t = "".join(content.split())
            douban_item["introduce"] = content_t

        douban_item["star"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/div[@class='star']/span[2]/text()").extract_first()
        douban_item["evaluate"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/div[@class='star']/span[4]/text()").extract_first()
```
9. 获取多页数据


```
 # 获取下一页链接
next_link = response.xpath("//span[@class='next']/link/@href").extract()

# 翻页
if next_link:
    next_link = next_link[0]
    yield scrapy.Request("https://movie.douban.com/top250"+next_link,callback=self.parse)

```
##### 参考实例代码

douban_spider.py
```
# -*- coding: utf-8 -*-
import scrapy
from spider.items import SpiderItem

class DoubanSpiderSpider(scrapy.Spider):
    # 爬虫名字
    name = 'douban_spider'
    # 允许的域名
    allowed_domains = ['movie.douban.com']
    # 入口url,扔到调度器里面去
    start_urls = ['https://movie.douban.com/top250']

    # 解析返回的数据
    def parse(self, response):
        # print(response.text)
        movie_list = response.xpath("//div[@class='article']/ol[@class='grid_view']/li")
        for i_item in movie_list:
            douban_item = SpiderItem()
            douban_item["serial_number"] = i_item.xpath(".//div[@class='pic']/em/text()").extract_first()
            douban_item["movie_name"] = i_item.xpath(".//div[@class='info']//div[@class='hd']/a/span[1]/text()").extract_first()
            douban_item["describe"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/p[@class='quote']/span/text()").extract_first()
            contents = i_item.xpath(".//div[@class='info']//div[@class='bd']/p[1]/text()").extract()

            for content in contents:
                content_t = "".join(content.split())
                douban_item["introduce"] = content_t

            douban_item["star"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/div[@class='star']/span[2]/text()").extract_first()
            douban_item["evaluate"] = i_item.xpath(".//div[@class='info']//div[@class='bd']/div[@class='star']/span[4]/text()").extract_first()
            # print(douban_item)
            yield douban_item
        # 获取下一页链接
        next_link = response.xpath("//span[@class='next']/link/@href").extract()

        # 翻页
        if next_link:
            next_link = next_link[0]
            yield scrapy.Request("https://movie.douban.com/top250"+next_link,callback=self.parse)

```
item.py


```
import scrapy


class SpiderItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    #序号
    serial_number = scrapy.Field()
    # 电影名
    movie_name = scrapy.Field()
    # 电影介绍
    introduce = scrapy.Field()
    # 星级
    star = scrapy.Field()
    # 电影评论数
    evaluate = scrapy.Field()
    # 电影描述
    describe = scrapy.Field()

```

##### 问题整理
