#include "RC663SimulationDataGenerator.h"
#include "RC663AnalyzerSettings.h"

RC663SimulationDataGenerator::RC663SimulationDataGenerator()
{
}

RC663SimulationDataGenerator::~RC663SimulationDataGenerator()
{
}

void RC663SimulationDataGenerator::Initialize( U32 simulation_sample_rate, RC663AnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mClockGenerator.Init( simulation_sample_rate / 10, simulation_sample_rate );

	if( settings->mMisoChannel != UNDEFINED_CHANNEL )
		mMiso = mRC663SimulationChannels.Add( settings->mMisoChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mMiso = NULL;
	
	if( settings->mMosiChannel != UNDEFINED_CHANNEL )
		mMosi = mRC663SimulationChannels.Add( settings->mMosiChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mMosi = NULL;

	mClock = mRC663SimulationChannels.Add( settings->mClockChannel, mSimulationSampleRateHz, mSettings->mClockInactiveState );

	if( settings->mEnableChannel != UNDEFINED_CHANNEL )
		mEnable = mRC663SimulationChannels.Add( settings->mEnableChannel, mSimulationSampleRateHz, Invert( mSettings->mEnableActiveState ) );
	else
		mEnable = NULL;

	mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle

	mValue = 0;
}

U32 RC663SimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mClock->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateRC663Transaction();

		mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle
	}

	*simulation_channels = mRC663SimulationChannels.GetArray();
	return mRC663SimulationChannels.GetCount();
}

void RC663SimulationDataGenerator::CreateRC663Transaction()
{
	if( mEnable != NULL )
		mEnable->Transition();

	mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
	
	if( mSettings->mDataValidEdge == AnalyzerEnums::LeadingEdge )
	{
		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;

		if( mEnable != NULL )
			mEnable->Transition();
	
		OutputWord_CPHA0( mValue, mValue+1 );
		mValue++;
	}else
	{
		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;

		if( mEnable != NULL )
			mEnable->Transition();
	
		OutputWord_CPHA1( mValue, mValue+1 );
		mValue++;
	}
}

void RC663SimulationDataGenerator::OutputWord_CPHA0( U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, 8 );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, 8 );

	U32 count = 8;
	for( U32 i=0; i<count; i++ )
	{
		if( mMosi != NULL )
			mMosi->TransitionIfNeeded( mosi_bits.GetNextBit() );

		if( mMiso != NULL )
			mMiso->TransitionIfNeeded( miso_bits.GetNextBit() );

		mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data invalid
	}

	if( mMosi != NULL )
		mMosi->TransitionIfNeeded( BIT_LOW );

	if( mMiso != NULL )
		mMiso->TransitionIfNeeded( BIT_LOW );

	mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}

void RC663SimulationDataGenerator::OutputWord_CPHA1( U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, 8 );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, 8 );

	U32 count = 8;
	for( U32 i=0; i<count; i++ )
	{
		mClock->Transition();  //data invalid
		if( mMosi != NULL )
			mMosi->TransitionIfNeeded( mosi_bits.GetNextBit() );
		if( mMiso != NULL )
			mMiso->TransitionIfNeeded( miso_bits.GetNextBit() );

		mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
	}

	if( mMosi != NULL )
		mMosi->TransitionIfNeeded( BIT_LOW );
	if( mMiso != NULL )
		mMiso->TransitionIfNeeded( BIT_LOW );

	mRC663SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}
