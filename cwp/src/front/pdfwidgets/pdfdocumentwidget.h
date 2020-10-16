#ifndef PDFDOCUMENTWIDGET_H
#define PDFDOCUMENTWIDGET_H

#include <QLabel>
#include <QRectF>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <poppler-qt4.h>
#else
#include <poppler-qt5.h>
#endif


class QRubberBand;

class PdfDocumentWidget : public QLabel
{
    Q_OBJECT

public:
    PdfDocumentWidget(QWidget *parent = 0);
    ~PdfDocumentWidget();
    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;

public slots:
    QRectF searchBackwards(const QString &text);
    QRectF searchForwards(const QString &text);
    bool setDocument(const QString &file_path);
    void setPage(int page = -1);
    void setScale(qreal scale);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void pageChanged(int current_page);
    void textSelected(const QString &text);

private:
    void selectedText(const QRectF &rect);
    void showPage(int page = -1);

    bool search(const Poppler::Page *page, const QString &text, QRectF &rect,
                Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitive,
                Poppler::Page::Rotation rotate = Poppler::Page::Rotate0) const;

    Poppler::Document *doc_;
    int current_page_;
    QPoint drag_position_;
    QRubberBand *rubber_band_;
    QRectF search_location_;
    qreal scale_factor_;
};

#endif
