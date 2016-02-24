/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TEncOpenCL.h
 * Author: augusto
 *
 * Created on February 3, 2016, 8:57 PM
 */

#ifndef TENCOPENCL_H
#define TENCOPENCL_H


#include "TLibCommon/TypeDef.h"
#include "TLibCommon/CommonDef.h"
#include "TLibCommon/TComMv.h"
#include "CL/cl.h"

#include <iostream>
#include <fstream>


class TEncOpenCL {
public:
    
	TEncOpenCL();
    ~TEncOpenCL();
    
	Bool					findDevice(Int device);
	Bool					compileKernel(const TChar *fileName);
	Bool					createBuffers(UInt i_maxCtuWidth, UInt i_maxCtuHeight, Int i_searchRange);
	Void					calcMotionVectors(Pel* pelCtu, Pel* pelSearch, Int i_iRefStride, Int i_iCtuStride, Int i_areaSize, TComMv* pcMvSrchRngLT, Bool fastSearch = false, MEFastSearchMethod search = MEFASTSEARCH_DIAMOND);

	Int						getDeviceId() { return deviceId; }
	Void					setDeviceId(Int i) { deviceId = i; }

	const TChar*			getDeviceInfo()		{ return info; }
	Distortion*				getRuiCost()		{ return ruiCosts; }
	Int*					getX()				{ return Xarray; }
	Int*					getY()				{ return Yarray; }

	Distortion*				getRuiCostPred()	{ return ruiCostsPred; }
	Int*					getXPred()			{ return XarrayPred; }
	Int*					getYPred()			{ return YarrayPred; }


	/* Lambda for calculate motion cost - Don't use cu_transquant_bypass */
	Void					setLambda(Double lambda) { m_lambda = (UInt)floor(65536.0 * sqrt(lambda)); } 
    
private:
    
// ====================================================================================================================
// Variable to configure OpenCL
// ====================================================================================================================
    cl_int                  errNum;                 ///< Indicate error 
    cl_uint                 numPlatforms;           ///< Number of platforms found in host
    cl_uint                 numDevices;             ///< Number of devices found that support OpenCL
    cl_platform_id*         platformIDs;            ///< Information of platforms found
    cl_device_id*           deviceIDs;              ///< Information of Devices found
    cl_context              context;                ///< Create OpenCL context 
    cl_command_queue        queue;                  ///< Create OpenCL Command Queue
    cl_program              program;                ///< Create OpenCL Program
    cl_kernel               kernelCalc;             ///< Create Kernel to calculate SAD
    cl_kernel               kernelCompare;          ///< Create Kernel to compare SAD
    std::size_t             paramValueSize;         ///< Info Device
    Int                     deviceId;               ///< Device Id
    TChar*                  info;                   ///< Info of OpenCL Device
    const TChar*            kernelSource;           ///< kernel Source Filename
	TComMv*                 searchPoints;           ///< array with ponits to performed fast search
	Int						searchLenght;

// ====================================================================================================================
// Buffer definition
// ====================================================================================================================
    
    // ========= Input Buffers ====================
    cl_mem                  pelCtuBuffer;           ///< Global Buffer of Pel CTU 
    cl_mem                  pelAreaBuffer;          ///< Global Buffer of Pel Search Area

	 // ============== Output Buffers =================
   
    cl_mem                  sadBuffer;              ///< minimum SAD Buffer
    cl_mem                  XarrayBuffer;           ///< pos X of minimum SAD Buffer
    cl_mem                  YarrayBuffer;           ///< pos Y of minimum SAD Buffer
    cl_mem                  ruiCostBuffer;          ///< Rui Cost of Blocks Buffer
    cl_mem                  tempSadBuffer;
       
    // ========== Auxiliar Buffers=====================
   
    
    // ======== Input Data =======================
   
    Pel*                    pelCtuArray;            ///< Pointer to pixel of initial position of CTU   
    Pel*                    pelSearchArray;         ///< Pointer to pixel of initial position of Search area
    cl_int                  iRefStride;             ///< Reference image stride
    cl_int                  iCtuStride;             ///< Ctu Stride
    
    // ========== Auxiliars Data ===================
    Pel*                    srchAreaPtr;         
    
    
    // ========= Output Arrays =========================
    Int*                    Xarray;                 ///< Pos X of minimun SAD blocks
    Int*                    Yarray;                 ///< Pos X of minimun SAD blocks
    Distortion*             minSad;                 ///< Minimum SAD blocks
    Distortion*             ruiCosts;                ///< Rui Cost of Blocks

	// ========= Output Arrays for Look-ahead =========================
	Int*                    XarrayPred;                 ///< Pos X of minimun SAD blocks
	Int*                    YarrayPred;                 ///< Pos X of minimun SAD blocks
	Distortion*             minSadPred;                 ///< Minimum SAD blocks
	Distortion*             ruiCostsPred;                ///< Rui Cost of Blocks
    
    // ========= Values =========================

    cl_uint                 maxCtuWidth;
    cl_uint                 maxCtuHeight;
    cl_int                  searchRange;
    cl_int                  areaSize; 
    UInt                    m_lambda;
    
	Bool					checkError(cl_int error, const TChar * message);
    
	Void					checkErrorAndExit();

	Void					xFillSADBuffer();
	Void					xResetArrays();
    Int						xCalcPoints(Int searchRange, TComMv * points, MEFastSearchMethod search);
	
	
};

#endif /* TENCOPENCL_H */

