import urllib, time, random
from datetime import datetime
from urllib.request import urlopen, Request

class PBIPost():


    def __init__(self, pbi_api_url):
        self.url = pbi_api_url

    def post(self, timestamp, cputemp, cpuutil,heatsink, cover, starttime, ambienttemp):
        self.data = '[{{"timestamp" :"{0}", \
"tempurature" :{1}, \
"cpu_utilisation" :{2}, \
"heat_sink_on" :"{3}", \
"cover_on" :"{4}", \
"start_time" :"{5}", \
"ambient_temp" :{6} \
}}]'.format(timestamp, cputemp, cpuutil,heatsink, cover, starttime, ambienttemp).encode('ascii')
        req = Request(self.url, self.data)
        self.response = urlopen(req)
