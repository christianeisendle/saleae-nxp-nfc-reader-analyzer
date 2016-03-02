#include "Pn5180AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "Pn5180Analyzer.h"
#include "Pn5180AnalyzerSettings.h"
#include <iostream>
#include <sstream>

#pragma warning(disable: 4996) //warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
#define NUM_INSTRUCTIONS 26U
const char* Pn5180InstructionCodes[NUM_INSTRUCTIONS] = 
{
	"SET_INSTR_WRITE_REGISTER",
	"SET_INSTR_WRITE_REGISTER_OR_MASK",
	"SET_INSTR_WRITE_REGISTER_AND_MASK",
	"SET_INSTR_WRITE_REGISTER_MULTIPLE",
	"GET_INSTR_READ_REGISTER",
	"GET_INSTR_READ_REGISTER_MULTIPLE",
	"SET_INSTR_WRITE_E2PROM",
	"GET_INSTR_READ_E2PROM",
	"SET_INSTR_WRITE_TX_DATA",
	"SET_INSTR_SEND_DATA",
	"GET_INSTR_RETRIEVE_RX_DATA",
	"SET_INSTR_SWITCH_MODE",
	"GET_INSTR_MFC_AUTHENTICATE",
	"SET_INSTR_EPC_GEN2_INVENTORY",
	"SET_INSTR_EPC_GEN2_RESUME_INVENTORY",
	"GET_INSTR_EPC_GEN2_RETRIEVE_INVENTORY_RESULT_SIZE",
	"GET_INSTR_EPC_GEN2_RETRIEVE_INVENTORY_RESULT",
	"SET_INSTR_LOAD_RF_CONFIGURATION",
	"SET_INSTR_UPDATE_RF_CONFIGURATION",
	"GET_INSTR_RETRIEVE_RF_CONFIGURATION_SIZE",
	"GET_INSTR_RETRIEVE_RF_CONFIGURATION",
	"SET_INSTR_CONFIGURE_TESTBUS (RFU)",
	"SET_INSTR_RF_ON",
	"SET_INSTR_RF_OFF",
	"SET_CONFIGURE_TESTBUS_DIGITAL",
	"SET_CONFIGURE_TESTBUS_ANALOG"

};

Pn5180AnalyzerResults::Pn5180AnalyzerResults( Pn5180Analyzer* analyzer, Pn5180AnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer ),
	mCnt( 0 ),
	mOldFrameId( 0 )
{
}

Pn5180AnalyzerResults::~Pn5180AnalyzerResults()
{
}

void Pn5180AnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );
	if(  frame.mFlags & Pn5180_INSTRUCTION_FLAG )
	{
		if ( channel == mSettings->mEnableChannel )
		{
			if (frame.mData1 >= NUM_INSTRUCTIONS)
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
				AddResultString( number_str );
				AddResultString( "Unknown Instruction" );
			}
			else
			{
				AddResultString( Pn5180InstructionCodes[frame.mData1] );
			}
		}
	}
	else
	{
		if( ( frame.mFlags & Pn5180_ERROR_FLAG ) == 0 )
		{
			if( channel == mSettings->mMosiChannel )
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
				AddResultString( number_str );

			}else
			{
				char number_str[128];
				AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, number_str, 128 );
				AddResultString( number_str );
			}
		}else
		{
				AddResultString( "Error" );
				AddResultString( "Settings mismatch" );
				AddResultString( "The initial (idle) state of the CLK line does not match the settings." );
		}
	}
}

void Pn5180AnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 /*export_type_user_id*/ )
{
	//export_type_user_id is only important if we have more than one export type.


	std::stringstream ss;
	void* f = AnalyzerHelpers::StartFile( file );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	ss << "Time [s],Packet ID,MOSI,MISO" << std::endl;

	bool mosi_used = true;
	bool miso_used = true;

	if( mSettings->mMosiChannel == UNDEFINED_CHANNEL )
		mosi_used = false;

	if( mSettings->mMisoChannel == UNDEFINED_CHANNEL )
		miso_used = false;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );

		if( ( frame.mFlags & Pn5180_ERROR_FLAG ) != 0 )
			continue;
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char mosi_str[128] = "";
		if( mosi_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, mosi_str, 128 );

		char miso_str[128] = "";
		if( miso_used == true )
			AnalyzerHelpers::GetNumberString( frame.mData2, display_base, 8, miso_str, 128 );

		U64 packet_id = GetPacketContainingFrameSequential( i ); 
		if( packet_id != INVALID_RESULT_INDEX )
			ss << time_str << "," << packet_id << "," << mosi_str << "," << miso_str << std::endl;
		else
			ss << time_str << ",," << mosi_str << "," << miso_str << std::endl;  //it's ok for a frame not to be included in a packet.
	
		AnalyzerHelpers::AppendToFile( (U8*)ss.str().c_str(), ss.str().length(), f );
		ss.str( std::string() );
							
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			AnalyzerHelpers::EndFile( f );
			return;
		}
	}

	UpdateExportProgressAndCheckForCancel( num_frames, num_frames );
	AnalyzerHelpers::EndFile( f );
}

void Pn5180AnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
    ClearTabularText();
	Frame frame = GetFrame( frame_index );

	std::stringstream ss;

	if(  frame.mFlags & Pn5180_INSTRUCTION_FLAG  )
	{
		char instr_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, instr_str, 128 );

		if (frame.mData1 >= NUM_INSTRUCTIONS)
		{
			ss << "Unknown Instruction (";
			ss << instr_str;
			ss << ")";
		}
		else
		{
			ss << Pn5180InstructionCodes[frame.mData1];
		}

	}
	else
	{
		char mosi_str[128];
		
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, mosi_str, 128 );

		if(  frame.mFlags & Pn5180_NEW_FRAME_FLAG  )
		{
			mCnt = 0;
			ss << "Instruction: ";
			ss << mosi_str;
		}
		else
		{
			ss << "Payload[";
			ss << mCnt-1;
			ss << "]: ";
			ss << mosi_str;
			if (mOldFrameId < frame_index)
			{
				mCnt++;
				mOldFrameId = frame_index;
			}
		}
	}

	AddTabularText( ss.str().c_str() );
}

void Pn5180AnalyzerResults::GeneratePacketTabularText( U64 /*packet_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );
}

void Pn5180AnalyzerResults::GenerateTransactionTabularText( U64 /*transaction_id*/, DisplayBase /*display_base*/ )  //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString( "not supported" );

}
