#include "SatViewerWidgetFactory.hpp"
#include "SatViewerWidget.hpp"

using namespace SigDigger;

// ================================= SATVIEWER WIDGET =================================

SatViewerWidget::SatViewerWidget(SatViewerWidgetFactory *factory, UIMediator *mediator, QWidget *parent) : ToolWidget(factory, mediator, parent), ui(new Ui::AudioPanel) {
    this->setProperty("collapsed", this->panelConfig->collapsed);

}

SatViewerWidget::~SatViewerWidget() {
    
}

bool AudioWidget::event(QEvent *event)
{
  if (event->type() == QEvent::DynamicPropertyChange) {
    QDynamicPropertyChangeEvent *const propEvent =
        static_cast<QDynamicPropertyChangeEvent*>(event);
    QString propName = propEvent->propertyName();
    if (propName == "collapsed")
      this->panelConfig->collapsed = this->property("collapsed").value<bool>();
  }

  return QWidget::event(event);
}

void AudioWidget::setQth(Suscan::Location const &qth) {
    this->fcDialog->setQth(qth.getQth());
}