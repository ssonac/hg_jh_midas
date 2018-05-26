/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "diagramscene.h"
#include "arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

#include <vector>
#include <utility>
#include<math.h>
#include<queue>
#include<iostream>

using namespace std;

typedef struct{
    int x1,y1,x2,y2,state;
}node;
vector < node  > v;

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

//! [6]

int X1,X2,Y1,Y2;
QLine Q1,Q2;
int arr[1030][1030];  // -1 : 시작점과 끝점, 0: 빈거 1: 벽
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
           return;

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:
            qDebug ("message %s, says: %d","Insert Item",myMode);
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            break;
//! [6] //! [7]
        case InsertLine:
            qDebug ("message %s, says: %d","InsertLine",myMode);
            // pos_first = mouseEvent->scenePos().rx();
             //pos_second = mouseEvent->scenePos().rx();
             //qDebug ("message %d says: %d",a++," start");
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
           // addItem(line);
            addItem(line);
            break;

        case DrawLine:
            X1=mouseEvent->scenePos().rx(),Y1=mouseEvent->scenePos().ry();
            break;

//! [7] //! [8]
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
//! [8] //! [9]
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if ((myMode == InsertLine && line != 0)||(myMode == DrawLine && line != 0)) {
      //  QLineF newLine(line->line().p1(), mouseEvent->scenePos());
      //  line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

int paintArr(int x1,int y1,int x2,int y2, int state){//벽 색을 칠해주는 함
  // qDebug("%d %d %d %d",x1,y1,x2,y2);
    qDebug("p1");
    int flag;
    flag=1;
    if(state==1){  // - line
        for(int i=x1+1;i<x2;i++){
            if(arr[y1][i]==1){
               return 0;
            }
        }
    }
    else{
       for(int i=y1+1;i<y2;i++){
            if(arr[i][x1]==1){
                return 0;
            }
        }

    }
    qDebug("p2");
    if(state==1){
        arr[y1][x1]=-1;
        arr[y1][x2]=-1;
        for(int i=x1+1;i<x2;i++){
            arr[y1][i]=1;
            //qDebug("%d",arr[y1][i]);

        }
    }
    else{
        arr[y1][x1]=-1;
        arr[y2][x1]=-1;
        for(int i=y1+1;i<y2;i++){
            arr[i][x1]=1;
        }
    }
    return 1;
}
int ulen(int x1,int y1, int x2,int y2){
    return (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

void magnetic(int x1, int y1,int x2,int y2,int state, int offset){//좌표 두개와 마그넷 파워 설

    int diff=1000000000;
    int flag;
    int tx,ty;
    flag=0;
    tx=ty=0;
    for(int i=0;i<v.size();i++){
        int len[4];
        len[0]= ulen(x1,y1,v[i].x1,v[i].y1); //up
        len[1]= ulen(x1,y1,v[i].x2,v[i].y2); //down
        len[2]= ulen(x2,y2,v[i].x1,v[i].y1);
        len[3]= ulen(x2,y2,v[i].x2,v[i].y2);
        for(int j=0;j<4;j++){
              qDebug("len%d: %d diff : %d",j,len[j],diff);
                if(diff>len[j] && len[j]<offset){//
                   diff=len[j];
                   if((j%2)==0)  tx=v[i].x1,ty=v[i].y1;
                   else tx=v[i].x2,ty=v[i].y2;
                   flag=j+1;
                }
        }
    }
   // qDebug("diff flag : %d %d",diff,flag);
    if(state==1){
        if(flag==1 ||flag==2){
               X1=tx,Y1=ty;
        }
        else if(flag==3||flag==4){
               X2=tx,Y2=ty;
               Y1=ty;
        }
    }
    if(state==2){
        if(flag==1||flag==2){
            X1=tx,Y1=ty;
        }
        else if(flag==3||flag==4){
            X2=tx,Y2=ty;
            X1=tx;
        }
    }
  // qDebug("mag x1y1x2y2 %d %d %d %d",(int)X1,(int)Y1,(int)X2,(int)Y1);
}
int dr[4]={1,-1,0,0};
int dc[4]={0,0,1,-1};
int check[1020][1020];
void bfs(int r, int c){

    queue< pair<int , int> > q;
    q.push({r,c});
    check[r][c]=1;

    while(!q.empty()){
        int hr,hc;
        hr= q.front().first;
        hc = q.front().second;
        q.pop();

        for(int k=0;k<4;k++){
            int nr,nc;
            nr = hr + dr[k], nc = hc+dc[k];
            if(0<=nr && nr<1000&&0<=nc&&nc<1000&&check[nr][nc]==0 && arr[nr][nc]==0){
                q.push({hr,hc});
                check[nr][nc]=1;
            }
        }
    }
}

int checkclose(){
   for(int i=0;i<1020;i++)for(int j=0;j<1020;j++) check[i][j]=0;

   int cnt=0;
   for(int j=0;j<1000;j++){
       for(int i=0;i<1000;i++){
           if(check[i][j]==0 && arr[i][j]==0){
                bfs(i,j);
                cnt++;
           }
          if(arr[i][j]==1&&check[i+1][j]==1)
               return 0;
       }
   }
   qDebug("cnt %d",cnt);
   return 1;
}
//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
     X2=mouseEvent->scenePos().rx(),Y2=mouseEvent->scenePos().ry();//놓는좌표
     int diffx = X2-X1;
     int diffy = Y2-Y1;
     if(diffx<0) diffx=-diffx;
     if(diffy<0) diffy=-diffy;
     if(diffx>diffy){ //  -line
            if(X1>X2){ //x1<x2
                int  temp;
                temp= X2;
                X2=X1;
                X1=temp;
            }
            qDebug(" - no ma x1y1x2y2 %d %d %d %d",X1,Y1,X2,Y1);
            magnetic(X1,Y1,X2,Y1,1,150);

            if(paintArr(X1,Y1,X2,Y1,1)==1){
                line = new QGraphicsLineItem(QLineF(X1,Y1,X2,Y1));
                line->setPen(QPen(Qt::black, 2));
                addItem(line);
                v.push_back({X1,Y1,X2,Y1,1});
                qDebug("why?");
            }
        }
     else{  // | line
            if(Y1>Y2){  //y1<y2
                int  temp;
                temp= Y2;
                Y2=Y1;
                Y1=temp;
            }
            qDebug(" | no ma x1,y1,x2,y2 %d %d %d %d",X1,Y1,X1,Y2);
           magnetic(X1,Y1,X1,Y2,2,100);

            if(paintArr(X1,Y1,X1,Y2,0)==1){
                line = new QGraphicsLineItem(QLineF(X1,Y1,X1,Y2));
                line->setPen(QPen(Qt::black, 2));
                addItem(line);
                v.push_back({X1,Y1,X1,Y2,2});//y1이 더 작은데 위에있음
                qDebug("why?");
            }
        }
     int closed;
        closed= checkclose();
        qDebug("closed %d",closed);

    if (line == 0 && myMode == InsertLine) {
    //if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();

        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]
