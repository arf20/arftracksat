#include "SatViewerWidgetFactory.hpp"
#include "SatViewerWidget.hpp"

using namespace SigDigger;

const char *
SatViewerWidgetFactory::name(void) const
{
  return "SatViewerWidget";
}

ToolWidget *
SatViewerWidgetFactory::make(UIMediator *mediator)
{
  return new SatViewerWidgetFactory(this, mediator);
}

SatViewerWidgetFactory::SatViewerWidgetFactory(Suscan::Plugin *plugin) :
  ToolWidgetFactory(plugin) { }

std::string
SatViewerWidgetFactory::getTitle() const
{
  return "Satellite Viewer";
}