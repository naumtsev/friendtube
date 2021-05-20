# -*- coding: utf-8 -*-
from flask import Flask
import requests
from bs4 import BeautifulSoup
import json
from flask import request

app = Flask(__name__)

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
                res['status'] = 'ok'
                return json.dumps(res)
        return {"status": "Adaptive video was not found"}
    except:
        return {"status": "Error"}


@app.route('/get_url', methods=['POST'])
def index():
    data = json.loads(request.data)
    return getStreamUrl(data['url'])


if __name__ == '__main__':
	app.run(port=1234)
