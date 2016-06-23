#ifndef RC663_SIMULATION_DATA_GENERATOR
#define RC663_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class RC663AnalyzerSettings;

class RC663SimulationDataGenerator
{
   public:
	   RC663SimulationDataGenerator();
	   ~RC663SimulationDataGenerator();

	   void Initialize( U32 simulation_sample_rate, RC663AnalyzerSettings* settings );
	   U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

   protected:
	   RC663AnalyzerSettings* mSettings;
	   U32 mSimulationSampleRateHz;
	   U64 mValue;

   protected: //RC663 specific
	   ClockGenerator mClockGenerator;

	   void CreateRC663Transaction();
	   void OutputWord_CPHA0( U64 mosi_data, U64 miso_data );
	   void OutputWord_CPHA1( U64 mosi_data, U64 miso_data );


	   SimulationChannelDescriptorGroup mRC663SimulationChannels;
	   SimulationChannelDescriptor* mMiso;
	   SimulationChannelDescriptor* mMosi;
	   SimulationChannelDescriptor* mClock;
	   SimulationChannelDescriptor* mEnable;
};
#endif //RC663_SIMULATION_DATA_GENERATOR
