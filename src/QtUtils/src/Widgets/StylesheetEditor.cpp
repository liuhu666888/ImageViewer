/*
   Copyright (C) 2017-2018 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `QtUtils' library.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "StylesheetEditor.h"
#include "StylesheetEditor_p.h"

#include <algorithm>

#include <QApplication>
#include <QEvent>
#include <QStyle>
#include <QPainter>
#include <QFile>
#include <QFont>
#include <QFontInfo>
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

namespace {

const int LINE_NUMBER_AREA_MARGIN = 3;
const int CODE_EDITOR_TAB_WIDTH   = 4;
const int WINDOW_DEFAULT_WIDTH    = 640;
const int WINDOW_DEFAULT_HEIGHT   = 480;

const QString INDENT_STRING = QString().fill(QChar::fromLatin1(' '), CODE_EDITOR_TAB_WIDTH);

static QFont getMonospaceFont()
{
    QFont font;
    font.setFamily(QString::fromLatin1("monospace"));
    if(QFontInfo(font).fixedPitch())
        return font;
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
    font.setStyleHint(QFont::Monospace);
    if(QFontInfo(font).fixedPitch())
        return font;
#endif
    font.setFamily(QString::fromLatin1("Courier New"));
    if(QFontInfo(font).fixedPitch())
        return font;
    font.setFamily(QString::fromLatin1("courier"));
    if(QFontInfo(font).fixedPitch())
        return font;
    font.setStyleHint(QFont::TypeWriter);
    if(QFontInfo(font).fixedPitch())
        return font;
    font.setFixedPitch(true);
    return font;
}

} // namespace

namespace StylesheetEditorImpl {

// ====================================================================================================

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_lineNumberArea(new LineNumberArea(this))
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumbersAreaWidth()));
    connect(this, SIGNAL(updateRequest(const QRect&, int)), this,SLOT(updateLineNumbersArea(const QRect&, int)));
    updateLineNumbersAreaWidth();
}

int CodeEditor::lineNumbersAreaWidth()
{
    int digits = 1;
    for(int max = qMax(1, blockCount()); max >= 10; max /= 10)
        digits++;
    return 2 * LINE_NUMBER_AREA_MARGIN + fontMetrics().width(QChar::fromLatin1('M')) * digits;
}

void CodeEditor::updateLineNumbersAreaWidth()
{
    setViewportMargins(lineNumbersAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumbersArea(const QRect &rect, int dy)
{
    if(dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    if(rect.contains(viewport()->rect()))
        updateLineNumbersAreaWidth();
}

void CodeEditor::showSearchDialog()
{
    if(m_searchDialog.isNull())
        m_searchDialog = new SearchDialog(this);
    const bool dialogIsHidden = m_searchDialog->isHidden();
    m_searchDialog->show();
    m_searchDialog->raise();
    m_searchDialog->activateWindow();
    if(dialogIsHidden)
    {
        const QSize dialogHalfSize = m_searchDialog->frameSize() / 2;
        const QPoint dialogLocalPos = geometry().center() - QPoint(dialogHalfSize.width(), dialogHalfSize.height());
        m_searchDialog->move(mapToGlobal(dialogLocalPos));
    }
}

bool CodeEditor::toggleCommentSelection()
{
    QTextCursor cursor = textCursor();
    if(!cursor.hasSelection())
        return false;

    int startPos = cursor.anchor();
    int endPos = cursor.position();
    if(startPos > endPos)
        std::swap(startPos, endPos);

    static const QString beginComment = QString::fromLatin1("/*");
    static const QString endComment = QString::fromLatin1("*/");

    cursor.setPosition(startPos, QTextCursor::MoveAnchor);
    cursor.setPosition(startPos + beginComment.length(), QTextCursor::KeepAnchor);
    const QString startText = cursor.selectedText();

    cursor.setPosition(endPos, QTextCursor::MoveAnchor);
    cursor.setPosition(endPos - endComment.length(), QTextCursor::KeepAnchor);
    const QString endText = cursor.selectedText();

    cursor.beginEditBlock();
    if(startText == beginComment && endText == endComment)
    {
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(startPos + beginComment.length(), QTextCursor::KeepAnchor);
        cursor.removeSelectedText();

        cursor.setPosition(endPos - beginComment.length(), QTextCursor::MoveAnchor);
        cursor.setPosition(endPos - beginComment.length() - endComment.length(), QTextCursor::KeepAnchor);
        cursor.removeSelectedText();

        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(endPos - beginComment.length() - endComment.length(), QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.insertText(beginComment);

        cursor.setPosition(endPos + beginComment.length(), QTextCursor::MoveAnchor);
        cursor.insertText(endComment);

        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(endPos + beginComment.length() + endComment.length(), QTextCursor::KeepAnchor);
    }
    cursor.endEditBlock();

    setTextCursor(cursor);
    return true;
}

bool CodeEditor::indentSelectedLines(IndentOperation operation)
{
    QTextCursor cursor = textCursor();
    if(!cursor.hasSelection())
        return false;

    int startPos = cursor.anchor();
    int endPos = cursor.position();
    if(startPos > endPos)
        std::swap(startPos, endPos);

    cursor.setPosition(startPos, QTextCursor::MoveAnchor);
    if(cursor.atBlockEnd())
    {
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    }
    const int startBlock = cursor.block().blockNumber();
    startPos = cursor.anchor();

    cursor.setPosition(endPos, QTextCursor::MoveAnchor);
    if(cursor.atBlockStart())
    {
        cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
    }
    const int endBlock = cursor.block().blockNumber();
    endPos = cursor.anchor();

    if(startPos >= endPos)
        return false;

    cursor.setPosition(startPos, QTextCursor::MoveAnchor);
    cursor.beginEditBlock();
    for(int i = 0, blocksDelta = endBlock - startBlock; i <= blocksDelta; ++i)
    {
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
        switch(operation)
        {
        case INDENT_INCREASE_LEVEL:
            if(!cursor.block().text().simplified().isEmpty())
                cursor.insertText(INDENT_STRING);
            break;
        case INDENT_DECREASE_LEVEL:
            if(cursor.block().text().startsWith(INDENT_STRING))
            {
                cursor.setPosition(cursor.position() + INDENT_STRING.length(), QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
            }
            break;
        }
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
    }
    cursor.endEditBlock();

    cursor.setPosition(startPos, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    while(cursor.block().blockNumber() < endBlock)
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    setTextCursor(cursor);
    return true;
}

bool CodeEditor::insertIndentSequence()
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
        return false;

    cursor.beginEditBlock();
    cursor.insertText(INDENT_STRING);
    cursor.endEditBlock();
    setTextCursor(cursor);
    return true;
}

bool CodeEditor::removeIndentSequence(RemoveDirection direction)
{
    QTextCursor cursor = textCursor();
    if(cursor.hasSelection())
        return false;

    int offset = 0;
    switch(direction)
    {
    case DIRECTION_BACKWARD:
        offset = - INDENT_STRING.length();
        break;
    case DIRECTION_FORWARD:
        offset = INDENT_STRING.length();
        break;
    }
    cursor.setPosition(cursor.position() + offset, QTextCursor::KeepAnchor);
    if(cursor.selectedText() != INDENT_STRING)
        return false;

    cursor.beginEditBlock();
    cursor.removeSelectedText();
    cursor.endEditBlock();
    setTextCursor(cursor);
    return true;
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    const QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumbersAreaWidth(), cr.height()));
}

void CodeEditor::changeEvent(QEvent *event)
{
    QPlainTextEdit::changeEvent(event);
    if(event->type() == QEvent::StyleChange)
        setTabStopWidth(CODE_EDITOR_TAB_WIDTH * fontMetrics().width(QChar::fromLatin1('M')));
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Find))
        showSearchDialog();
    else if(event->matches(QKeySequence::Save))
        emit applyRequested();
    else if((event->key() == Qt::Key_Slash || event->key() == Qt::Key_Question) && event->modifiers().testFlag(Qt::ControlModifier) && toggleCommentSelection())
        return;
    else if(((event->key() == Qt::Key_Tab && event->modifiers().testFlag(Qt::ShiftModifier)) || (event->key() == Qt::Key_Backtab)) && indentSelectedLines(INDENT_DECREASE_LEVEL))
        return;
    else if(event->key() == Qt::Key_Tab && indentSelectedLines(INDENT_INCREASE_LEVEL))
        return;
    else if(event->key() == Qt::Key_Tab && insertIndentSequence())
        return;
    else if(event->key() == Qt::Key_Delete && removeIndentSequence(DIRECTION_FORWARD))
        return;
    else if(event->key() == Qt::Key_Backspace && removeIndentSequence(DIRECTION_BACKWARD))
        return;
    else
        QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::hideEvent(QHideEvent *event)
{
    if(!m_searchDialog.isNull())
        m_searchDialog->close();
    QPlainTextEdit::hideEvent(event);
}

void CodeEditor::repaintLineNumbersArea(const QRect &rect)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(rect, Qt::lightGray);
    painter.setPen(Qt::black);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while(block.isValid() && top <= rect.bottom())
    {
        blockNumber++;
        if(block.isVisible() && bottom >= rect.top())
        {
            const QRect textRect = QRect(0, top, m_lineNumberArea->width() - LINE_NUMBER_AREA_MARGIN, fontMetrics().height());
            painter.drawText(textRect, Qt::AlignRight, QString::number(blockNumber));
        }
        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
    }
}

// ====================================================================================================

LineNumberArea::LineNumberArea(CodeEditor *editor)
    : QWidget(editor)
    , m_codeEditor(editor)
{}

QSize LineNumberArea::sizeHint() const
{
    return QSize(m_codeEditor->lineNumbersAreaWidth(), m_codeEditor->sizeHint().height());
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_codeEditor->repaintLineNumbersArea(event->rect());
}

// ====================================================================================================

SearchDialog::SearchDialog(CodeEditor *editor)
    : QDialog(editor)
    , m_codeEditor(editor)
    , m_searchLineEdit(new QLineEdit(this))
    , m_searchPushButton(new QPushButton(QString::fromLatin1("Find"), this))
    , m_findBackwardCheckBox(new QCheckBox(QString::fromLatin1("Find Backward"), this))
    , m_findCaseSensitivelyCheckBox(new QCheckBox(QString::fromLatin1("Find Case Sensitively"), this))
    , m_findWholeWordsCheckBox(new QCheckBox(QString::fromLatin1("Find Whole Words"), this))
{
    setWindowTitle(QString::fromLatin1("Search"));
    setWindowFlags(Qt::Dialog
                   | Qt::MSWindowsFixedSizeDialogHint
                   | Qt::CustomizeWindowHint
                   | Qt::WindowTitleHint
                   | Qt::WindowSystemMenuHint
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
                   | Qt::WindowCloseButtonHint
#endif
                   );
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_searchLineEdit);
    layout->addWidget(m_findBackwardCheckBox);
    layout->addWidget(m_findCaseSensitivelyCheckBox);
    layout->addWidget(m_findWholeWordsCheckBox);
    layout->addWidget(m_searchPushButton);
    connect(m_searchPushButton, SIGNAL(clicked(bool)), this, SLOT(onSearchClicked()));
}

void SearchDialog::onSearchClicked()
{
    const QString searchString = m_searchLineEdit->text();
    if(searchString.isEmpty())
        return;
    QTextDocument *document = m_codeEditor->document();
    QTextCursor cursor = m_codeEditor->textCursor();
    QTextDocument::FindFlags flags;
    if(m_findBackwardCheckBox->isChecked())
        flags |= QTextDocument::FindBackward;
    if(m_findCaseSensitivelyCheckBox->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if(m_findWholeWordsCheckBox->isChecked())
        flags |= QTextDocument::FindWholeWords;
    QTextCursor newCursor = document->find(searchString, cursor, flags);
    if(!newCursor.isNull())
        m_codeEditor->setTextCursor(newCursor);
}

void SearchDialog::showEvent(QShowEvent *event)
{
    m_searchLineEdit->setFocus();
    QDialog::showEvent(event);
}

// ====================================================================================================

} // namespace StylesheetEditorImpl

typedef StylesheetEditorImpl::CodeEditor CodeEditor;

struct StylesheetEditor::Impl
{
    const QString originalStyleSheet;

    QCheckBox *autoApplyCheckBox;
    QCheckBox *protectCheckBox;
    QPushButton *searchButton;
    CodeEditor *codeEditor;
    QPushButton *resetButton;
    QPushButton *applyButton;
    QPushButton *readButton;

    Impl(StylesheetEditor *stylesheetEditor)
        : originalStyleSheet(qApp->styleSheet())
        , autoApplyCheckBox(new QCheckBox(QString::fromLatin1("Auto apply style")))
        , protectCheckBox(new QCheckBox(QString::fromLatin1("Protect stylesheet editor")))
        , searchButton(new QPushButton(QString::fromLatin1("Search"), stylesheetEditor))
        , codeEditor(new CodeEditor(stylesheetEditor))
        , resetButton(new QPushButton(QString::fromLatin1("Reset style"), stylesheetEditor))
        , applyButton(new QPushButton(QString::fromLatin1("Apply style"), stylesheetEditor))
        , readButton(new QPushButton(QString::fromLatin1("Read style"), stylesheetEditor))
    {
        QGridLayout *layout = new QGridLayout(stylesheetEditor);

        layout->addWidget(autoApplyCheckBox, 0, 0);
        layout->addWidget(protectCheckBox, 0, 1);
        layout->addItem(new QSpacerItem(0, 0), 0, 2);
        layout->addWidget(searchButton, 0, 3);

        layout->addWidget(codeEditor, 1, 0, 1, 4);

        layout->addWidget(resetButton, 2, 0);
        layout->addWidget(applyButton, 2, 1, 1, 2);
        layout->addWidget(readButton, 2, 3);
    }
};

StylesheetEditor::StylesheetEditor(QWidget *parent)
    : QDialog(parent)
    , m_impl(new Impl(this))
{
    resize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
    setWindowFlags(Qt::Window);

    m_impl->codeEditor->setPlainText(m_impl->originalStyleSheet);
    m_impl->protectCheckBox->setChecked(true);
    updateProtection();

    connect(m_impl->searchButton, SIGNAL(clicked(bool)), m_impl->codeEditor, SLOT(showSearchDialog()));
    connect(m_impl->resetButton, SIGNAL(clicked(bool)), this, SLOT(resetStyleSheet()));
    connect(m_impl->applyButton, SIGNAL(clicked(bool)), this, SLOT(applyStylesheet()));
    connect(m_impl->readButton, SIGNAL(clicked(bool)), this, SLOT(readStyleSheet()));
    connect(m_impl->protectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateProtection()));
    connect(m_impl->autoApplyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateAutoApply()));
    connect(m_impl->codeEditor, SIGNAL(applyRequested()), this, SLOT(applyStylesheet()));

    m_impl->codeEditor->setFocus();
}

StylesheetEditor::~StylesheetEditor()
{}

bool StylesheetEditor::isProtected() const
{
    return m_impl->protectCheckBox->isChecked();
}

void StylesheetEditor::setProtected(bool isProtected)
{
    m_impl->protectCheckBox->setChecked(isProtected);
}

void StylesheetEditor::applyStylesheet()
{
    qApp->setStyleSheet(m_impl->codeEditor->toPlainText());
    qApp->style()->unpolish(qApp);
    qApp->style()->polish(qApp);
    const QWidgetList allWidgets = qApp->allWidgets();
    for(QWidgetList::ConstIterator it = allWidgets.constBegin(), itEnd = allWidgets.constEnd(); it != itEnd; ++it)
    {
        (*it)->update();
        (*it)->repaint();
    }
    m_impl->codeEditor->setFont(getMonospaceFont());
}

void StylesheetEditor::resetStyleSheet()
{
    m_impl->codeEditor->setPlainText(m_impl->originalStyleSheet);
    applyStylesheet();
}

void StylesheetEditor::readStyleSheet()
{
    m_impl->codeEditor->setPlainText(qApp->styleSheet());
}

void StylesheetEditor::updateProtection()
{
    QString styleSheet;
    if(m_impl->protectCheckBox->isChecked())
    {
        QFile style(QString::fromLatin1(":/style/StylesheetEditor.qss"));
        style.open(QIODevice::ReadOnly);
        styleSheet = QString::fromUtf8(style.readAll());
    }
    setStyleSheet(styleSheet);
    m_impl->codeEditor->setFont(getMonospaceFont());
}

void StylesheetEditor::updateAutoApply()
{
    if(m_impl->autoApplyCheckBox->isChecked())
        connect(m_impl->codeEditor, SIGNAL(textChanged()), this, SLOT(applyStylesheet()));
    else
        disconnect(m_impl->codeEditor, SIGNAL(textChanged()), this, SLOT(applyStylesheet()));
}

