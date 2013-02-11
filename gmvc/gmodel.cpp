#include "gmodel.h"

gModel::gModel(QObject *parent) :
        QAbstractListModel(parent)
{
}

QVariant gModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role == Qt::DecorationRole)
         return QIcon(pixmaps.value(index.row()).scaled(60, 60,
                          Qt::KeepAspectRatio, Qt::SmoothTransformation));
     /*else if (role == Qt::UserRole)
         return pixmaps.value(index.row());
     else if (role == Qt::UserRole + 1)
         return locations.value(index.row());*/

     return QVariant();
 }

 void gModel::addPiece(const QPixmap &pixmap, const QPoint &location)
 {
     /*int row;
     if (int(2.0*qrand()/(RAND_MAX+1.0)) == 1)
         row = 0;
     else
         row = pixmaps.size();*/
     int row = pixmaps.size();

     beginInsertRows(QModelIndex(), row, row);
     pixmaps.insert(row, pixmap);
     //locations.insert(row, location);
     endInsertRows();
 }

 /*Qt::ItemFlags gModel::flags(const QModelIndex &index) const
 {
     if (index.isValid())
         return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

     return Qt::ItemIsDropEnabled;
 }*/

 /*bool gModel::removeRows(int row, int count, const QModelIndex &parent)
 {
     if (parent.isValid())
         return false;

     if (row >= pixmaps.size() || row + count <= 0)
         return false;

     int beginRow = qMax(0, row);
     int endRow = qMin(row + count - 1, pixmaps.size() - 1);

     beginRemoveRows(parent, beginRow, endRow);

     while (beginRow <= endRow) {
         pixmaps.removeAt(beginRow);
         locations.removeAt(beginRow);
         ++beginRow;
     }

     endRemoveRows();
     return true;
 }*/

 /*QStringList gModel::mimeTypes() const
 {
     QStringList types;
     types << "image/x-puzzle-piece";
     return types;
 }*/

 /*QMimeData *gModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
             QPixmap pixmap = qVariantValue<QPixmap>(data(index, Qt::UserRole));
             QPoint location = data(index, Qt::UserRole+1).toPoint();
             stream << pixmap << location;
         }
     }

     mimeData->setData("image/x-puzzle-piece", encodedData);
     return mimeData;
 }*/

 /*bool gModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                int row, int column, const QModelIndex &parent)
 {
     if (!data->hasFormat("image/x-puzzle-piece"))
         return false;

     if (action == Qt::IgnoreAction)
         return true;

     if (column > 0)
         return false;

     int endRow;

     if (!parent.isValid()) {
         if (row < 0)
             endRow = pixmaps.size();
         else
             endRow = qMin(row, pixmaps.size());
     } else
         endRow = parent.row();

     QByteArray encodedData = data->data("image/x-puzzle-piece");
     QDataStream stream(&encodedData, QIODevice::ReadOnly);

     while (!stream.atEnd()) {
         QPixmap pixmap;
         QPoint location;
         stream >> pixmap >> location;

         beginInsertRows(QModelIndex(), endRow, endRow);
         pixmaps.insert(endRow, pixmap);
         locations.insert(endRow, location);
         endInsertRows();

         ++endRow;
     }

     return true;
 }*/

 int gModel::rowCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return 0;
     else
         return pixmaps.size();
 }

 /*Qt::DropActions gModel::supportedDropActions() const
 {
     return Qt::CopyAction | Qt::MoveAction;
 }*/

/* void gModel::addPieces(const QPixmap& pixmap)
 {
     beginRemoveRows(QModelIndex(), 0, 24);
     pixmaps.clear();
     locations.clear();
     endRemoveRows();
     for (int y = 0; y < 5; ++y) {
         for (int x = 0; x < 5; ++x) {
             QPixmap pieceImage = pixmap.copy(x*80, y*80, 80, 80);
             addPiece(pieceImage, QPoint(x, y));
         }
     }
 }*/
