/* **********************************************************************
 * NFS World - Tuning-Calculator                                        *
 * Copyright (C) 2011-2012 dragoniac                                    *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program.  If not, see http://www.gnu.org/licenses/.  *
 ********************************************************************** */

#include "CCar.h"
#include "NFSWCarData.h"

#include <QMessageBox>

#include "CPart.h"
#include "../utils.h"
#include "../Parser/IniParameters.h"

//==========================================================================================================================
//====== PRIVATE ===========================================================================================================
//==========================================================================================================================

CCar::Acquirable CCar::ID2Acquirable(const QString& _Text)
{
	QString s = _Text.toLower();

	if( s.compare("igc") == 0 || s.compare("0") == 0 )
		return forIGC;
	else if( s.compare("boost") == 0 || s.compare("1") == 0 )
		return forBoost;
	else if( s.compare("rental") == 0 || s.compare("2") == 0 )
		return asBoostRental;
	else if( s.compare("topup") == 0 || s.compare("3") == 0 )
		return asTopUp;
	else if( s.compare("retired") == 0 || s.compare("4") == 0 )
		return isRetired;
	else if( s.compare("vip") == 0 )
		return forVIPs;

	return notAvailable;
}

//==========================================================================================================================
//====== PROTECTED =========================================================================================================
//==========================================================================================================================

//==========================================================================================================================
//====== CONSTRUCTORS ======================================================================================================
//==========================================================================================================================

CCar::CCar()
{
	_brand = "";
	_type = "";
	_topspeed = 0;
	_acceleration = 0;
	_handling = 0;
	_hasNFSWPerformanceModel = false;
	for(int i = 0; i < 4; ++i)
	{
		_nfswTopspeed[i] = 0.0f;
		_nfswAcceleration[i] = 0.0f;
		_nfswHandling[i] = 0.0f;
	}
	_images.clear();
	_id = 0;
	_value = 0;
	_acquirable = notAvailable;
	_isReworked = false;
}
CCar::CCar(const QString& _File)
{
	Load(_File);
}
CCar::~CCar()
{
	_images.clear();
}

//==========================================================================================================================
//====== PUBLIC ============================================================================================================
//==========================================================================================================================

bool CCar::Load(const QString& _File)
{
	_images.clear();

	ini::Parameters file;

	if( !file.load(_File) )
		return false;

	bool ok = true;

	QString r = "";

	if( !file.read< QString >(CARBRAND, _brand, "") )
	{
		r += QString(" ") + CARBRAND;
		ok = false;
	}

	if( !file.read< QString >(CARTYPE, _type, "") )
	{
		r += QString(" ") + CARTYPE;
		ok = false;
	}

	if( !file.read< QString >(SPEEDAPICARBRAND, _speedAPIBrand, "") )
	{
		_speedAPIBrand = _brand;
	}

	if( !file.read< QString >(SPEEDAPICARTYPE, _speedAPIType, "") )
	{
		_speedAPIType = _type;
	}

	tValue t, a, h;

	if( !file.read< tValue >(istr(aTOPSPEED), t, 0) )
	{
		r += QString(" ") + str(aTOPSPEED);
		ok = false;
	}

	if( !file.read< tValue >(istr(aACCELERATION), a, 0) )
	{
		r += QString(" ") + str(aACCELERATION);
		ok = false;
	}

	if( !file.read< tValue >(istr(aHANDLING), h, 0) )
	{
		r += QString(" ") + str(aHANDLING);
		ok = false;
	}
	_topspeed = t;
	_acceleration = a;
	_handling = h;

	_hasNFSWPerformanceModel = true;
	for(int i = 0; i < 4; ++i)
	{
		if( !file.read< tValueF >(QString("NFSWTopSpeed%1").arg(i), _nfswTopspeed[i], 0.0f) )
			_hasNFSWPerformanceModel = false;
		if( !file.read< tValueF >(QString("NFSWAcceleration%1").arg(i), _nfswAcceleration[i], 0.0f) )
			_hasNFSWPerformanceModel = false;
		if( !file.read< tValueF >(QString("NFSWHandling%1").arg(i), _nfswHandling[i], 0.0f) )
			_hasNFSWPerformanceModel = false;
	}

	if( !file.read< tID >(CARID, _id, 0) )
	{
		r += QString(" ") + CARID;
		ok = false;
	}

	// GitHub/source builds keep the recovered 12-value table compiled in.
	// Values present in a .car file still take precedence.
	if( !_hasNFSWPerformanceModel )
		_hasNFSWPerformanceModel = NFSWCarData::Lookup(_id, _nfswTopspeed, _nfswAcceleration, _nfswHandling);

	QString id;
	if( !file.read< QString >(CARACQUIRABLE, id, "") )
	{
		r += QString(" ") + CARACQUIRABLE;
		ok = false;
	}
	_acquirable = ID2Acquirable(id);

	if( !file.read< tIGC >(CARIGCCOST, _value, 0) )
	{
		r += QString(" ") + CARIGCCOST;
		ok = false;
	}

	int reworked = 0;
	if( !file.read< int >(CARREWORKED, reworked, 0) )
	{
		r += QString(" ") + CARREWORKED;
		ok = false;
	}
	_isReworked = ( reworked != 0 );

	if( r.length() > 0 )
		r = "\n(" + r + " )";

	if( !ok )
		QMessageBox::warning(0, QMessageBox::tr("Warning"),
				QMessageBox::tr("Could not read all data from '%1'.%2").arg(_File).arg(r));

	return ok;
}

const QString& CCar::Brand() const
{
	return _brand;
}
const QString& CCar::Type() const
{
	return _type;
}
const QString& CCar::SpeedAPIBrand() const
{
	return _speedAPIBrand;
}
const QString& CCar::SpeedAPIType() const
{
	return _speedAPIType;
}
const tValue& CCar::Topspeed() const
{
	return _topspeed;
}
const tValue& CCar::Acceleration() const
{
	return _acceleration;
}
const tValue& CCar::Handling() const
{
	return _handling;
}
const tValue& CCar::Attribute(const EAttribute& _Attribute) const
{
	switch( _Attribute )
	{
		case ( aTOPSPEED ):
			return Topspeed();
			break;
		case ( aACCELERATION ):
			return Acceleration();
			break;
		case ( aHANDLING ):
			return Handling();
			break;
		default:
			return Topspeed();
	}

	return Topspeed();
}
bool CCar::HasNFSWPerformanceModel() const
{
	return _hasNFSWPerformanceModel;
}
const tValueF& CCar::NFSWBase(const EAttribute& _Attribute, const int& _Index) const
{
	int idx = _Index;
	if( idx < 0 ) idx = 0;
	if( idx > 3 ) idx = 3;

	switch( _Attribute )
	{
		case ( aACCELERATION ): return _nfswAcceleration[idx];
		case ( aHANDLING ): return _nfswHandling[idx];
		case ( aTOPSPEED ):
		default: return _nfswTopspeed[idx];
	}
}
const QString& CCar::Image(const int& _Index) const
{
	if( _Index < 0 || _images.size() <= _Index )
		return _images.at(0);

	return _images.at(_Index);
}
const CCar::Acquirable& CCar::IsAcquirable() const
{
	return _acquirable;
}
QString CCar::AcquirableImageName() const
{
	switch( _acquirable )
	{
		case ( forIGC ):
			return "igc.png";
			break;
		case ( forBoost ):
			return "boost.png";
			break;
		case ( asBoostRental ):
			return "rental.png";
			break;
		case ( asTopUp ):
			return "topup.png";
			break;
		case ( isRetired ):
			return "retired.png";
			break;
		case ( forVIPs ):
			return "vip.png";
			break;
		default:
			return "unbuyable.png";
			break;
	}

	return "unbuyable.png";
}
void CCar::ClearImages()
{
	_images.clear();
}
void CCar::AddCarImage(const QString& _Image)
{
	if( !_images.contains(_Image) )
		_images.append(_Image);
}
void CCar::SetCarImage(const QString& _Image)
{
	_images.clear();
	AddCarImage(_Image);
}
int CCar::ImageCount() const
{
	return _images.size();
}
QStringList::const_iterator CCar::FirstCarImage() const
{
	return _images.begin();
}
QStringList::const_iterator CCar::LastCarImage() const
{
	return _images.end() - 1;
}
QStringList::const_iterator CCar::FindCarImage(const QString& _Image) const
{
	for(QStringList::const_iterator i = _images.begin(); i != _images.end(); ++i)
	{
		if( *i == _Image )
			return i;
	}

	return FirstCarImage();
}
void CCar::NextCarImage(QStringList::const_iterator& _Idx) const
{
	++_Idx;

	if( _Idx == _images.end() )
		--_Idx;
}
void CCar::PrevCarImage(QStringList::const_iterator& _Idx) const
{
	if( _Idx != _images.begin() )
		--_Idx;
}
const tID& CCar::ID() const
{
	return _id;
}
const tIGC& CCar::Cost() const
{
	return _value;
}
tIGC CCar::SaleValue() const
{
	return Cost() / 4;
}
QString CCar::Name() const
{
	return Brand() + " " + Type();
}
QString CCar::SpeedAPIName() const
{
	return SpeedAPIBrand() + " " + SpeedAPIType();
}
bool CCar::IsReworked() const
{
	return _isReworked;
}
void CCar::MakeNotTCKable()
{
	while( _id < TCKLIMIT )
		_id += TCKLIMIT;
}
