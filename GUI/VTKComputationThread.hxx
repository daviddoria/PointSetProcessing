#include <iostream>

#include <vtkPolyData.h>

template<typename TFilter>
VTKComputationThread<TFilter>::VTKComputationThread()
{
  this->Filter = NULL;
}

template<typename TFilter>
void VTKComputationThread<TFilter>::start()
{
  std::cout << "VTKComputationThread::start()" << std::endl;

  if(this->Filter)
    {
    this->Filter->Update();
    }
  else
    {
    std::cerr << "No filter set in thread." << std::endl;
    }

  // When the function is finished, end the thread
  emit finished();
}

template<typename TFilter>
void VTKComputationThread<TFilter>::SetFilter(TFilter* filter)
{
  this->Filter = filter;
  this->Filter->AddObserver(this->Filter->IterateEvent, this, &VTKComputationThread<TFilter>::IterateCallbackFunction);
}

template<typename TFilter>
void VTKComputationThread<TFilter>::IterateCallbackFunction(vtkObject* caller, long unsigned int eventId, void* callData)
{
  int currentPoint = reinterpret_cast<int*>(callData)[0];
  //std::cout << "Emitting " << currentPoint << " from IterateCallbackFunction." << std::endl;
  if(currentPoint % 10 == 0 || currentPoint == vtkPolyData::SafeDownCast(this->Filter->GetInput())->GetNumberOfPoints() - 1)
    {
    emit progressUpdate(currentPoint);
    }
}
