#include <iostream>

template<typename TFilter>
VTKComputationThread<TFilter>::VTKComputationThread()
{
  this->Filter = NULL;
}

template<typename TFilter>
void VTKComputationThread<TFilter>::run()
{
  std::cout << "ProgressThread::run()" << std::endl;

  // When the thread is started, emit the signal to start the marquee progress bar
  emit StartProgressBarSignal();

  if(this->Filter)
    {
    this->Filter->Update();
    }
  else
    {
    std::cerr << "No filter set in thread." << std::endl;
    }

  // When the function is finished, end the thread
  exit();
}

template<typename TFilter>
void VTKComputationThread<TFilter>::exit()
{
  std::cout << "ProgressThread::exit()" << std::endl;
  
  // When the thread is stopped, emit the signal to stop the marquee progress bar
  emit StopProgressBarSignal();
}

template<typename TFilter>
void VTKComputationThread<TFilter>::SetFilter(TFilter* filter)
{
  this->Filter = filter;
}
