#include "mywidget.h"
#include "ui_mywidget.h"
#include <QFile>
#include <QFileDialog>
MyWidget::MyWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->book->setColumnCount(4);
    QStringList colTotal;
    colTotal<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->book->setHorizontalHeaderLabels(colTotal);

}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_insertButton_clicked()
{
    QTableWidgetItem *col1, *col2, *col3, *col4;
    col1=new QTableWidgetItem(ui->lineEdit_1->text());
    col2=new QTableWidgetItem(ui->lineEdit_2->text());
    col3=new QTableWidgetItem(ui->lineEdit_3->text());
    col4=new QTableWidgetItem(ui->lineEdit_4->text());
    int row = ui->book->rowCount();
    qDebug()<<row;
    ui->book->insertRow(row);
    ui->book->setItem(row,0,col1);
    ui->book->setItem(row,1,col2);
    ui->book->setItem(row,2,col3);
    ui->book->setItem(row,3,col4);
}

void CbWrite(QString fileName,QString str){
    QFile outFile(fileName);
    if(!outFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug()<<"Open File failed";
        return;
    }
    QTextStream out(&outFile);
    out.setEncoding(QStringConverter::System);
    out<<str;
    outFile.flush();
    outFile.close();
}


void MyWidget::on_exportButton_clicked()
{
    QString fileName;
    QString saveStr;
    fileName= QFileDialog::getSaveFileName(this,"匯出通訊錄",".","csv(*.csv);;text(*.txt)");

    for(int i=0;i<ui->book->rowCount();i++){
        for(int j=0;j<ui->book->columnCount();j++){
            saveStr+=ui->book->item(i,j)->text()+",";
        }
        saveStr+="\n";

    }
    CbWrite(fileName,saveStr);
}

void MyWidget::on_importButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "匯出通訊錄",".","csv(*.csv);;text(*.txt)");

    if(fileName.isEmpty()){
        return;
    }

    QFile inFile(fileName);
    if(!inFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "錯誤";
        return;
    }

    QTextStream in(&inFile);
    in.setEncoding(QStringConverter::System);

    ui->book->setRowCount(0);

    while(!in.atEnd()){
        QString line = in.readLine();
        if(line.isEmpty()) continue;
        QStringList fields = line.split(",");
        if(fields.size() >= 4){
            int currentRow = ui->book->rowCount();
            ui->book->insertRow(currentRow);
            for(int col = 0; col < 4; col++){
                QTableWidgetItem *newItem = new QTableWidgetItem(fields[col]);
                ui->book->setItem(currentRow, col, newItem);
            }
        }
    }
    inFile.close();
}


