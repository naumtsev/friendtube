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
        res = {}

        res['duration'] = data['resources'][rootResourceId]['meta']['etime']
        res['name'] = data['resources'][rootResourceId]['name']
        for v in videos:
            if v['dimension'] == 'adaptive':
                res['stream_url'] = v['url']
                return json.dumps(res)
        return 'adaptive video was not found'
    except:
        return "error"



parser = argparse.ArgumentParser()
parser.add_argument('--link')
args = parser.parse_args()
print(getStreamUrl(args.link), end='')

# example
#print(getStreamUrl('https://disk.yandex.ru/i/maQWX1KvkNJlhQ'))