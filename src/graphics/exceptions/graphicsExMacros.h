#pragma once
#include "graphicsException.h"

// hResult variable should be in scope if you want to use theese
 
#define GFX_EXCEPT_NOINFO(hr) HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_NOINFO(hrCall) if (FAILED(hrCall)) throw HResultException( __LINE__,__FILE__,hr )
#define GFX_THROW_REMOVED_EXCEPT(hr) HResultException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define INFOMANAGER(gfx) HRESULT hr; auto& infoManager_ = gfx.getInfoManager()
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#define GFX_THROW_INFO(hrcall) infoManager_.set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager_.getLog() )
#define GFX_THROW_INFO_ONLY(call) infoManager_.set(); (call); {auto v = infoManager_.getLog(); if (!v.empty()) {throw InfoException(__LINE__, __FILE__, v);}}
#else
#define INFOMAN(gfx) HRESULT hr
#define GFX_EXCEPT(hr) HResultException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

