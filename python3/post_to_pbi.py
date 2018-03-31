import urllib, time, random
from datetime import datetime
from urllib.request import urlopen, Request

def post(pbi_api_url, timestamp, cpu_temp, cpu_util, cpu_freq, amb_temp, amb_humid, heatsink, cover, starttime):
    data = '[{{"timestamp" :"{0}", \
"cpu_temp" :{1}, \
"cpu_util" :{2},\
"cpu_freq" :{3},\
"amb_temp" :{4},\
"amb_humid" :{5},\
"heat_sink_on" :"{6}", \
"cover_on" :"{7}", \
"start_time" :"{8}"\
}}]'.format(timestamp, cpu_temp, cpu_util, cpu_freq, amb_temp, amb_humid, heatsink, cover, starttime).encode('ascii')

    print(data)

    req = Request(pbi_api_url, data)
    try:
        response = urlopen(req, timeout=10)
    except urllib.error.HTTPError as e:
        print("HTTP Error: {0} - {1}".format(e.code, e.reason))
    except TimeoutError:
        print("Request timed out")
        
