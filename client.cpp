#include <QMessageBox>
#include <QString>
#include <cstring>
#include "client.h"
#include "ui_client.h"
#include <QPainter>
int currentColumnL;//��ǰ����
int currentRowH;//��ǰ����
//int cutmessage(QString *buf)
//{
//     QString cutstr = ":#";
//     unsigned char num[4];
//     num[0]=buf->indexOf(cutstr);
//     num[1]=buf->indexOf(cutstr,num[0]+1);
//     num[2]=buf->indexOf(cutstr,num[1]+1);
//     num[3]=buf->indexOf(cutstr,num[2]+1);
//}
client::client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);

    ui->clientSendpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (false);

    ui->stoprecordpushButton->setEnabled(false);
    tcp=NULL;
    flagRecord=0;
    currentColumnL=0;
    currentRowH=0;
    message=tr("<span style=\"color:red\">%1</span>");//�����,���Ǹ��ĵ���messagetextBrowser��receive����ɫ
}

client::~client()
{
    delete tcp;
    delete ui;
}


//�ͻ��˷�����Ϣ
void client::clientSendMessage ()
{
    QByteArray data;//�ֽ�����
    data.append(ui->clientMessagelineEdit->text());
    tcp->write(data);
    ui->messagetextBrowser->insertPlainText (tr("send message: %1 \n").arg (QString(data)));//�ٷֺ�֮���ǰٷ�֮һ������
}
//qinchu
void client::on_cCleanpushButton_clicked()
{
    ui->messagetextBrowser->setText ("");
    ui->atableWidget->clearContents();
    //ui->atableWidget->setItem(0,0,new QTableWidgetItem("Polling ID"));
    for(int icom=1;icom<=comIndex;++icom)
    {
        ui->atableWidget->setItem(icom,0,new QTableWidgetItem("(100,100)"));
    }
}

//�ͻ������Ӱ�ť�ۺ���
void client::on_connectpushButton_clicked()
{
    serverIP = ui->clientIPlineEdit->text ();
    clientPort = ui->clientPortlineEdit->text ();
    if(serverIP.isEmpty () || clientPort.isEmpty ())//�������ڱȽ϶˿ں�IP�Ƿ�Ϊ��
    {
        QMessageBox::warning (this, tr("Warnning"), tr("Server IP or port number cannot be empty!!!"));//������IP��˿ںŲ���Ϊ��
        return;
    }
//��һ��if��Ҫ�ҵ�TCP��ʲô������������������ɾ����
    if(tcp)
    {
        delete tcp;
    }
    tcp=new QTcpSocket(this);
    tcp->connectToHost (serverIP, clientPort.toInt ()); //���ӵ�����
    connect (tcp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect (tcp, SIGNAL(connected()), this, SLOT(updateClientStatusConnect())); //��������״̬
    //connect (tcp, SIGNAL(disconnected()), this, SLOT(updateClientStatusDisonnect())); //���¶Ͽ�����״̬
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
}

void client::on_disconnectpushButton_clicked()
{
    ui->clientMessagelineEdit->setText (tr("clientStop"));
    clientSendMessage ();
    ui->clientMessagelineEdit->setText (tr(""));
    ui->cStatuslabel->setText (tr("Connection disconnect"));//���ӶϿ�
    tcp->abort();//��ֹ��Ϣ
    delete tcp;
    tcp=NULL;//��ָ�룬��IF���
    ui->connectpushButton->setEnabled (true);
    ui->disconnectpushButton->setEnabled (false);
    ui->clientSendpushButton->setEnabled (false);


}

//�ͻ��˴�����ʾ
void client::displayError (QAbstractSocket::SocketError)
{
      QMessageBox::warning (this, tr("Warnning"), tcp->errorString ());
      tcp->close ();
      ui->connectpushButton->setEnabled (true);
      ui->disconnectpushButton->setEnabled (false);
      ui->clientSendpushButton->setEnabled (false);
}

void client::updateClientStatusConnect ()
{
    ui->cStatuslabel->setText (tr("Connected"));//������
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
    ui->clientSendpushButton->setEnabled (true);
}

//void client::updateClientStatusDisonnect()
//{
//    ui->cStatuslabel->setText (tr("������ֹͣ����"));
//    ui->connectpushButton->setEnabled (true);
//    ui->disconnectpushButton->setEnabled (false);
//    ui->clientSendpushButton->setEnabled (false);
//}

//��Ҫ�Լ���ӵĺ���
void client::updateStatus()
{
    ui->connectpushButton->setEnabled (false);
    ui->disconnectpushButton->setEnabled (true);
    ui->clientSendpushButton->setEnabled (true);
}

//�ͻ��˶�ȡ��Ϣ
void client::readMessage ()
{
    QByteArray data=tcp->readAll();
    message = QString(data);
    ui->messagetextBrowser->append(message.arg(QString(data)));//�յ���Ϣ:
    if(flagRecord)
    {
        //if(message.contains("WORNG:"))
        QString cutstr = "#";
        QString endstr = "=";
        QString startstr = "S#";
        int startnum;
//        QString startstrerror = "W#";
        int num[6];
        int currentColumnL_temp;
        while(1)
        {
            num[0]=message.indexOf(startstr,startnum);
            num[1]=message.indexOf(cutstr,num[0]+1);//��ȡ�Ƿ�Ϊ������ϢW/R return 1
            num[2]=message.indexOf(cutstr,num[1]+1);//��ȡID return 3
            num[3]=message.indexOf(cutstr,num[2]+1);//����dis return 5
            num[4]=message.indexOf(cutstr,num[3]+1);//����polling
            num[5]=message.indexOf(endstr,num[4]+1);//��������β

            startnum=num[5];
            QString AnchorRW=message.mid(num[1]+1,num[2]-num[1]-1);//W/R
            QString Anchorid=message.mid(num[2]+1,num[3]-num[2]-1);//id
            QString Anchordis=message.mid(num[3]+1,num[4]-num[3]-1);//dis
            QString Anchorpol=message.mid(num[4]+1,num[5]-num[4]-1);//poll_id
            QString AnchorEndFlag=message.mid(num[5]+1,4);//"+e+e"

            currentRowH = Anchorid.toInt();
            if(currentColumnL_temp!=Anchorpol.toInt())
            {
                currentColumnL +=1;//Anchorpol.toInt();
                if(currentColumnL>500)
                {
                    currentColumnL=1;
                }
            }
           currentColumnL_temp=Anchorpol.toInt();

            ui->atableWidget->setItem(currentRowH,currentColumnL,new QTableWidgetItem(Anchordis));
            ui->atableWidget->setItem(0,currentColumnL,new QTableWidgetItem(Anchorpol));
            num[0]=0;num[1]=0;num[2]=0;num[3]=0;num[4]=0;num[5]=0;
            if(AnchorEndFlag=="+e+e")
            {
                startnum=0;
                break;
            }
        }
//        ui->atableWidget->setItem(1,1,new QTableWidgetItem(Anchornum));
//        ui->atableWidget->setItem(2,1,new QTableWidgetItem(Anchordis));
//        ui->atableWidget->setItem(3,1,new QTableWidgetItem(Anchorpol));
    }
//    if(message.contains ("serverStop")) //����յ��ǿͻ��˶Ͽ����ӵ���Ϣ
//    {
//        ui->cStatuslabel->setText (tr("������ֹͣ����"));
//        ui->connectpushButton->setEnabled (true);
//        ui->disconnectpushButton->setEnabled (false);
//        ui->clientSendpushButton->setEnabled (false);
//       return;
//    }
    //ui->messagetextBrowser->append(message.arg(tr("&lt;&lt;Received"))+QString(data));//�յ���Ϣ:
//    ui->messagetextBrowser->append(message.arg(QString(data)));//�յ���Ϣ:

}

void client::on_clientSendpushButton_clicked()
{
    QByteArray data;
    data.append(ui->clientMessagelineEdit->text());
    tcp->write(data);
    ui->messagetextBrowser->insertPlainText (tr("send message: %1 \n").arg (QString(data)));
}
//��¼���ݰ�ť
void client::on_recordpushButton_clicked()
{
    comIndex=ui->basecomboBox->currentIndex();
//    QString comText=ui->basecomboBox->currentText();
//    ui->messagetextBrowser->insertPlainText (tr("anchornum: %1 \n").arg (comText));
    ui->recordpushButton->setEnabled(false);
    ui->stoprecordpushButton->setEnabled(true);

    ui->atableWidget->setRowCount(comIndex+2);
    ui->atableWidget->setColumnCount(502);
    ui->atableWidget->resize(400,250);

    QStringList header;
    header<<"Anchor coordinate";

    ui->atableWidget->setHorizontalHeaderLabels(header);
    QStringList headercol;
    headercol<<"Polling ID"<<"anchor 1"<<"anchor 2"<<"anchor 3"<<"anchor 4"<<"anchor 5"<<"anchor 6"<<"anchor 7";

    ui->atableWidget->setVerticalHeaderLabels(headercol);

    //ui->atableWidget->setItem(0,0,new QTableWidgetItem("Polling ID"));
    for(int icom=1;icom<=comIndex+1;++icom)
    {
        ui->atableWidget->setItem(icom,0,new QTableWidgetItem("(100,100)"));
    }
    flagRecord=1;
    currentColumnL=1;
    currentRowH=1;
}
void client::on_stoprecordpushButton_clicked()
{
    flagRecord=0;
    currentColumnL=0;
    currentRowH=0;

    ui->recordpushButton->setEnabled(true);
    ui->stoprecordpushButton->setEnabled(false);
}

void client::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /*!
         * Draw the border
         */
//        painter.drawRect(40, 100, width()-500, height()-500);	/**< if you don't subtract 1, you can not see the border on bottom and right. */
//        painter.fillRect(40, 100, width()-500, height()-500, Qt::red);
    painter.drawRect(40, 250, 300, 300);	/**< if you don't subtract 1, you can not see the border on bottom and right. */
    painter.fillRect(40, 250, 302, 300, Qt::white);
        /*!
         * Build the coordinate
         */

        QFontMetrics metrics = painter.fontMetrics();

       // int leftBearing = metrics.width("100") + 5; /**< 5 is the scale width. */
        int coordWidth = 300;//width() - 2*leftBearing;
        int coordHeight = 300;//height() - 4*metrics.height(); /**< just try */

        QStringList monthList;
        monthList << "0.0" << "100.0" << "200.0" << "300.0" << "400.0" << "500.0";

        QStringList valueList;
        valueList << "0.0" <<"100.0" << "200.0" << "300.0" << "400.0" << "500.0";

        painter.translate(40,550);//(leftBearing, coordHeight + 1.75*metrics.height()); /**< move center to left bottom */����ԭ��
        float deltaX = static_cast<float>(coordWidth)/(monthList.size()-1);
        float deltaY = static_cast<float>(coordHeight)/(valueList.size()-1);

        /*!
         * Draw the coordinate
         */
        painter.drawLine(0, 0, coordWidth, 0);
        for (int i = 0; i != monthList.size(); ++i)
        {
            int strLen = metrics.width(monthList.at(i));
            // scale
            painter.drawLine(deltaX*(i), 0, deltaX*(i), 4);
            // text
            painter.drawText(deltaX*i-strLen/2  ,2*metrics.height(), monthList.at(i));//+ (deltaX-strLen)/2
        }
        painter.drawText(250,-300,"unit:mm");
        painter.drawLine(0, 0, 0, -coordHeight);
        for (int i = 0; i != valueList.size(); ++i)
        {
            int deviation = metrics.height()/2 - metrics.descent();
            painter.drawLine(-4, -deltaY*i, 0, -deltaY*i);
            painter.drawText(-metrics.width(valueList.at(i))-4, -deltaY*i+deviation, valueList.at(i));
        }
//    painter.drawLine(QPointF(0,0),QPointF(100,100));
//    QPen pen; //����
//    pen.setColor(QColor(255, 0, 0));
//    QBrush brush(QColor(0, 255, 0, 125)); //��ˢ
//    painter.setPen(pen); //��ӻ���
//    painter.setBrush(brush); //��ӻ�ˢ
//    painter.drawRect(50, 50, 200, 100); //���ƾ���

//    QPen pen(Qt::DotLine);
//    QBrush brush(Qt::blue);
//    brush.setStyle(Qt::HorPattern);
//    painter.setPen(pen);
//    painter.setBrush(brush);
//    painter.drawRect(50,50,200,200);
//    QPainterPath path;
//    path.moveTo(50,250);
//    path.lineTo(50,230);

}
