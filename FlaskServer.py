#codeing: utf-8
from flask import Flask, render_template
app = Flask(__name__)

@app.route('/')
def health():
    return "Server is running."


@app.route('/FoodInfo')
def info():
    ticket_path = './ticket.txt'
    reccomend_path = './reccomend.txt'
    f = open (ticket_path, 'r')
    content1 = f.readlines()
    f.close
    #show reccomends
    f = open (reccomend_path, 'r')
    print (content1)
    #print (content2)
    #content2 = f.readlines()
    content2 = "Comming soon!" # <=== Change here.
    return render_template('info.html', infomation=content1, reccommend=content2)

if __name__ == "__main__":
    app.run(debug=False, host='0.0.0.0')
