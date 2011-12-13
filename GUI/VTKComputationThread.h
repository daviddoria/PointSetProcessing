#ifndef VTKComputationThread_H
#define VTKComputationThread_H

#include <QThread>

class FilterComputationThreadNonTemplated : public QThread
{
Q_OBJECT
signals:
  // This signal is emitted to start the progress bar
  void StartProgressBarSignal();

  // This signal is emitted to stop the progress bar
  void StopProgressBarSignal();

};

template <typename TFilter>
class VTKComputationThread : public FilterComputationThreadNonTemplated
{
  public:

  VTKComputationThread();

  // This function is called when the thread is started
  void run();

  // This function is called when the thread is stopped
  void exit();

  void SetFilter(TFilter* filter);

private:
  TFilter* Filter;

};

#include "VTKComputationThread.hxx"

#endif
