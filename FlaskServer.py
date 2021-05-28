#coding: utf-8
from flask import Flask, render_template
import os
app = Flask(__name__)

@app.route('/')
def health():
    return "Server is running.<br>==> Please access /FoodInfo ."


@app.route('/FoodInfo')
def info():
    ticket_path = './ticket.txt'
    reccomend_path = './recommend.txt'
    os.system("python3 /home/ec2-user/vending/sdpblseiretu.py")
    #os.system("/home/ec2-user/vending/forecast.out")
    f = open (ticket_path, 'r')
    scaned1 = f.readlines()
    content1 = ''
    for i in range(len(scaned1)):
        scaned1[i] = scaned1[i].replace('\n', '<br>')
        scaned1[i] = scaned1[i].replace('\t', ' &#009 ')
        content1 += scaned1[i]
    f.close
    #show reccomends
    f = open (reccomend_path, 'r')
    scaned2 = f.readlines()
    content2 = ''
    for i in range(len(scaned2)):
        scaned2[i] = scaned2[i].replace('\n', '<br>')
        scaned2[i] = scaned2[i].replace('\t', ' &#009 ')
        content2 += scaned2[i]
    return render_template('info.html', infomation=content1, reccomend=content2)

@app.route('/infomation')
def backup():
    dummy_ticket_path = './dummy_ticket.txt'
    dummy_reccomend_path = './dummy_recommend.txt'
    f = open (dummy_ticket_path, 'r')
    scaned1 = f.readlines()
    dummy_content1 = ''
    for i in range(len(scaned1)):
        scaned1[i] = scaned1[i].replace('\n', '<br>')
        scaned1[i] = scaned1[i].replace('\t', ' &#009 ')
        dummy_content1 += scaned1[i]
    f.close
    #show reccomends
    f = open (dummy_reccomend_path, 'r')
    scaned2 = f.readlines()
    dummy_content2 = ''
    for i in range(len(scaned2)):
        scaned2[i] = scaned2[i].replace('\n', '<br>')
        scaned2[i] = scaned2[i].replace('\t', ' &#009 ')
        dummy_content2 += scaned2[i]
    return render_template('backup.html', dummy_infomation=dummy_content1, dummy_reccomend=dummy_content2)

if __name__ == "__main__":
    app.run(debug=False, host='0.0.0.0')
