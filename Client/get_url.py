import requests
from bs4 import BeautifulSoup
import json
import argparse

def getStreamUrl(short_url):
    try:
        html_doc = requests.get(short_url).text
        soup = BeautifulSoup(html_doc, 'html.parser')
        data = json.loads(soup.find("body").find("script", attrs={"type": "application/json"}).next_element)
        rootResourceId = data['rootResourceId']
        videos = data['resources'][rootResourceId]['videoStreams']['videos']
        for v in videos:
            if v['dimension'] == 'adaptive':
                return v['url']
        return 'incorrect url'
    except:
        return "error"

parser = argparse.ArgumentParser()
parser.add_argument('--link')
args = parser.parse_args()
print(getStreamUrl(args.link), end='')
# example
#print(getStreamUrl('https://disk.yandex.ru/i/maQWX1KvkNJlhQ'))