#pragma once

#include <ToolWidgetFactory.h>

namespace SigDigger {
  class SatViewerWidgetFactory : public ToolWidgetFactory
  {
  public:
    // FeatureFactory overrides
    const char *name(void) const override;

    // ToolWidgetFactory overrides
    ToolWidget *make(UIMediator *) override;
    std::string getTitle() const override;

    SatViewerWidgetFactory(Suscan::Plugin *);
  };
}