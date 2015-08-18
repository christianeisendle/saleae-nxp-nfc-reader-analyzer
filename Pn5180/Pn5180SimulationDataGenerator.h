#ifndef Pn5180_SIMULATION_DATA_GENERATOR
#define Pn5180_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class Pn5180AnalyzerSettings;

class Pn5180SimulationDataGenerator
{
public:
	Pn5180SimulationDataGenerator();
	~Pn5180SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, Pn5180AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

protected:
	Pn5180AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	U64 mValue;

protected: //Pn5180 specific
	ClockGenerator mClockGenerator;

	void CreatePn5180Transaction();
	void OutputWord_CPHA0( U64 mosi_data, U64 miso_data );
	void OutputWord_CPHA1( U64 mosi_data, U64 miso_data );


	SimulationChannelDescriptorGroup mPn5180SimulationChannels;
	SimulationChannelDescriptor* mMiso;
	SimulationChannelDescriptor* mMosi;
	SimulationChannelDescriptor* mClock;
	SimulationChannelDescriptor* mEnable;
	SimulationChannelDescriptor* mBusy;
};
#endif //Pn5180_SIMULATION_DATA_GENERATOR
