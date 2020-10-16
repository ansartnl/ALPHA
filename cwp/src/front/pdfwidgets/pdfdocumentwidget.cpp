#include <QtGui>
#include <QRubberBand>

#include "pdfdocumentwidget.h"

PdfDocumentWidget::PdfDocumentWidget(QWidget *parent)
    : QLabel(parent),
	doc_(0),
	current_page_(-1),
	rubber_band_(0),
	scale_factor_(1.0)
{
    setAlignment(Qt::AlignCenter);
}

PdfDocumentWidget::~PdfDocumentWidget()
{
    delete doc_;
}

Poppler::Document *PdfDocumentWidget::document()
{
    return doc_;
}

QMatrix PdfDocumentWidget::matrix() const
{
    return QMatrix(scale_factor_ * physicalDpiX() / 72.0, 0,
                   0, scale_factor_ * physicalDpiY() / 72.0,
                   0, 0);
}

void PdfDocumentWidget::mousePressEvent(QMouseEvent *event)
{
    if (!doc_)
        return;

    drag_position_ = event->pos();
    if (!rubber_band_)
        rubber_band_ = new QRubberBand(QRubberBand::Rectangle, this);
    rubber_band_->setGeometry(QRect(drag_position_, QSize()));
    rubber_band_->show();
}

void PdfDocumentWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!doc_)
        return;

    rubber_band_->setGeometry(QRect(drag_position_, event->pos()).normalized());
}

void PdfDocumentWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (!doc_)
        return;

    if (!rubber_band_->size().isEmpty()) {
        // Correct for the margin around the image in the label.
        QRectF rect = QRectF(rubber_band_->pos(), rubber_band_->size());
        rect.moveLeft(rect.left() - (width() - pixmap()->width()) / 2.0);
        rect.moveTop(rect.top() - (height() - pixmap()->height()) / 2.0);
        selectedText(rect);
    }

    rubber_band_->hide();
}

qreal PdfDocumentWidget::scale() const
{
    return scale_factor_;
}

void PdfDocumentWidget::showPage(int page)
{
    if (page != -1 && page != current_page_ + 1) {
        current_page_ = page - 1;
        emit pageChanged(page);
    }

    QImage image = doc_->page(current_page_)
                      ->renderToImage(scale_factor_ * physicalDpiX(), scale_factor_ * physicalDpiY());

    if (!search_location_.isEmpty()) {
        QRect highlightRect = matrix().mapRect(search_location_).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
        painter.drawImage(highlightRect, highlight);
        painter.end();
    }

    setPixmap(QPixmap::fromImage(image));
}

QRectF PdfDocumentWidget::searchBackwards(const QString &text)
{
    QRectF old_location = search_location_;

    int page = current_page_;
    if (old_location.isNull())
        page -= 1;

    while (page > -1) {

        QList<QRectF> locations;
        search_location_ = QRectF();

        while (search(doc_->page(page), text, search_location_,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            if (search_location_ != old_location)
                locations.append(search_location_);
            else
                break;
        }

        int index = locations.indexOf(old_location);
        if (index == -1 && !locations.isEmpty()) {
            search_location_ = locations.last();
            showPage(page + 1);
            return search_location_;
        } else if (index > 0) {
            search_location_ = locations[index - 1];
            showPage(page + 1);
            return search_location_;
        }

        old_location = QRectF();
        page -= 1;
    }

    if (current_page_ == doc_->numPages() - 1)
        return QRectF();

    old_location = QRectF();
    page = doc_->numPages() - 1;

    while (page > current_page_) {

        QList<QRectF> locations;
        search_location_ = QRectF();

        while (search(doc_->page(page), text, search_location_,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            locations.append(search_location_);
        }

        if (!locations.isEmpty()) {
            search_location_ = locations.last();
            showPage(page + 1);
            return search_location_;
        }
        page -= 1;
    }

    return QRectF();
}

QRectF PdfDocumentWidget::searchForwards(const QString &text)
{
    int page = current_page_;
    while (page < doc_->numPages()) {

        if (search(doc_->page(page), text, search_location_,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!search_location_.isNull()) {
                showPage(page + 1);
                return search_location_;
            }
        }
        page += 1;
        search_location_ = QRectF();
    }

    page = 0;

    while (page < current_page_) {

        search_location_ = QRectF();

        if (search(doc_->page(page), text, search_location_,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {
            if (!search_location_.isNull()) {
                showPage(page + 1);
                return search_location_;
            }
        }
        page += 1;
    }

    return QRectF();
}

void PdfDocumentWidget::selectedText(const QRectF &rect)
{
    QRectF selected_rect = matrix().inverted().mapRect(rect);
    // QString text = doc->page(currentPage)->text(selectedRect);

    QString text;
    bool hadSpace = false;
    QPointF center;
    foreach (Poppler::TextBox *box, doc_->page(current_page_)->textList()) {
        if (selected_rect.intersects(box->boundingBox())) {
            if (hadSpace)
                text += " ";
            if (!text.isEmpty() && box->boundingBox().top() > center.y())
                text += "\n";
            text += box->text();
            hadSpace = box->hasSpaceAfter();
            center = box->boundingBox().center();
        }
    }

    if (!text.isEmpty())
        emit textSelected(text);
}

bool PdfDocumentWidget::setDocument(const QString &file)
{
    Poppler::Document *old_document = doc_;

    doc_ = Poppler::Document::load(file);
    if (doc_) {
        delete old_document;
        doc_->setRenderHint(Poppler::Document::Antialiasing);
        doc_->setRenderHint(Poppler::Document::TextAntialiasing);
        search_location_ = QRectF();
        current_page_ = -1;
        setPage(1);
    }
    return doc_ != 0;
}

void PdfDocumentWidget::setPage(int page)
{
    if (page != current_page_ + 1) {
        search_location_ = QRectF();
        showPage(page);
    }
}

void PdfDocumentWidget::setScale(qreal scale)
{
    if (scale_factor_ != scale) {
        scale_factor_ = scale;
        showPage();
    }
}

bool PdfDocumentWidget::search(const Poppler::Page *page, const QString &text, QRectF &rect,
                               Poppler::Page::SearchDirection direction, Poppler::Page::SearchMode caseSensitive,
                               Poppler::Page::Rotation rotate) const
{
    double left   = rect.left();
    double top    = rect.top();
    double right  = rect.right();
    double bottom = rect.bottom();

    bool found = page->search(text, left, top, right, bottom, direction, caseSensitive, rotate);

    rect.setLeft(left);
    rect.setTop(top);
    rect.setRight(right);
    rect.setBottom(bottom);

    return found;
}
