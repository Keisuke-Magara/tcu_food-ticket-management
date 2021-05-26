# coding: utf-8
import codecs
f=codecs.open('../ftp_files/tickets.info','r','utf-8')
g=codecs.open('ticket.txt','w','utf-8')
line=f.readline()
print('商品名','　　　　　　価格','　残り枚数')
g.writelines(['商品名','　　　　　価格','　　残り枚数','\n'])
while line:
	x=line.split()
	y=7-len(x[4])
	print(x[4],'　'*y,x[5],'円','残り',x[1],'枚')
	g.writelines([x[4],'　'*(y+1),x[5],'円','　','残り',x[1],'枚','\n'])
	line=f.readline()
	if line=='\n':
		break
line='\n'+f.readline()
print(line)
g.write(line)
f.close
g.close
