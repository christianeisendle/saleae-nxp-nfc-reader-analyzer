#include "Pn5180SimulationDataGenerator.h"
#include "Pn5180AnalyzerSettings.h"

Pn5180SimulationDataGenerator::Pn5180SimulationDataGenerator()
{
}

Pn5180SimulationDataGenerator::~Pn5180SimulationDataGenerator()
{
}

void Pn5180SimulationDataGenerator::Initialize( U32 simulation_sample_rate, Pn5180AnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mClockGenerator.Init( simulation_sample_rate / 10, simulation_sample_rate );

	if( settings->mMisoChannel != UNDEFINED_CHANNEL )
		mMiso = mPn5180SimulationChannels.Add( settings->mMisoChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mMiso = NULL;
	
	if( settings->mMosiChannel != UNDEFINED_CHANNEL )
		mMosi = mPn5180SimulationChannels.Add( settings->mMosiChannel, mSimulationSampleRateHz, BIT_LOW );
	else
		mMosi = NULL;

	mClock = mPn5180SimulationChannels.Add( settings->mClockChannel, mSimulationSampleRateHz, mSettings->mClockInactiveState );

	if( settings->mEnableChannel != UNDEFINED_CHANNEL )
		mEnable = mPn5180SimulationChannels.Add( settings->mEnableChannel, mSimulationSampleRateHz, Invert( mSettings->mEnableActiveState ) );
	else
		mEnable = NULL;

	mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle

	mValue = 0;
}

U32 Pn5180SimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mClock->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreatePn5180Transaction();

		mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle
	}

	*simulation_channels = mPn5180SimulationChannels.GetArray();
	return mPn5180SimulationChannels.GetCount();
}

void Pn5180SimulationDataGenerator::CreatePn5180Transaction()
{
	if( mEnable != NULL )
		mEnable->Transition();

	mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
	
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

void Pn5180SimulationDataGenerator::OutputWord_CPHA0( U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );

	U32 count = mSettings->mBitsPerTransfer;
	for( U32 i=0; i<count; i++ )
	{
		if( mMosi != NULL )
			mMosi->TransitionIfNeeded( mosi_bits.GetNextBit() );

		if( mMiso != NULL )
			mMiso->TransitionIfNeeded( miso_bits.GetNextBit() );

		mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data invalid
	}

	if( mMosi != NULL )
		mMosi->TransitionIfNeeded( BIT_LOW );

	if( mMiso != NULL )
		mMiso->TransitionIfNeeded( BIT_LOW );

	mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}

void Pn5180SimulationDataGenerator::OutputWord_CPHA1( U64 mosi_data, U64 miso_data )
{
	BitExtractor mosi_bits( mosi_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );
	BitExtractor miso_bits( miso_data, mSettings->mShiftOrder, mSettings->mBitsPerTransfer );

	U32 count = mSettings->mBitsPerTransfer;
	for( U32 i=0; i<count; i++ )
	{
		mClock->Transition();  //data invalid
		if( mMosi != NULL )
			mMosi->TransitionIfNeeded( mosi_bits.GetNextBit() );
		if( mMiso != NULL )
			mMiso->TransitionIfNeeded( miso_bits.GetNextBit() );

		mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
		mClock->Transition();  //data valid

		mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( .5 ) );
	}

	if( mMosi != NULL )
		mMosi->TransitionIfNeeded( BIT_LOW );
	if( mMiso != NULL )
		mMiso->TransitionIfNeeded( BIT_LOW );

	mPn5180SimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 2.0 ) );
}
