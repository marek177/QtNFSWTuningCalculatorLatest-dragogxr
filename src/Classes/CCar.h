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

#ifndef __CCAR_H__
#define __CCAR_H__

#include <QString>
#include <QStringList>

#include "EAttribute.h"
#include "CAttributes.h"
#include "TypeDefinitions.h"

class CPart;

#define CARBRAND "Brand"
#define CARTYPE "Type"
#define SPEEDAPICARBRAND "SpeedAPIBrand"
#define SPEEDAPICARTYPE "SpeedAPIType"
#define CARID "ID"
#define CARACQUIRABLE "Acquirable"
#define CARIGCCOST "Cost"
#define CARREWORKED "Reworked"

/**
 * @brief this class provides all data, that belongs to a car.
 * @author dragoniac
 */
class CCar
{
	public:
		enum Acquirable
		{
			forIGC = 0, forBoost = 1, asBoostRental = 2, asTopUp = 3, isRetired = 4, forVIPs = 5, notAvailable = 999
		};
	private:
		Acquirable ID2Acquirable(const QString& _Text);

	public:
		CCar();
		CCar(const QString& _File);
		virtual ~CCar();

		bool Load(const QString& _File);

		const QString& Brand() const;
		const QString& Type() const;
		const QString& SpeedAPIBrand() const;
		const QString& SpeedAPIType() const;
		const tValue& Topspeed() const;
		const tValue& Acceleration() const;
		const tValue& Handling() const;
		const tValue& Attribute(const EAttribute& _Attribute) const;

		// 12-value performance model used by the newer NFSW algorithm.
		bool HasNFSWPerformanceModel() const;
		const tValueF& NFSWBase(const EAttribute& _Attribute, const int& _Index) const;
		const QString& Image(const int& _Index = 0) const;
		const Acquirable& IsAcquirable() const;
		QString AcquirableImageName() const;
		void ClearImages();
		void AddCarImage(const QString& _Image);
		void SetCarImage(const QString& _Image);
		int ImageCount() const;
		QStringList::const_iterator FirstCarImage() const;
		QStringList::const_iterator LastCarImage() const;
		QStringList::const_iterator FindCarImage(const QString& _Image) const;

		void NextCarImage(QStringList::const_iterator& _Idx) const;
		void PrevCarImage(QStringList::const_iterator& _Idx) const;

		const tID& ID() const;
		const tIGC& Cost() const;
		tIGC SaleValue() const;
		QString Name() const;
		QString SpeedAPIName() const;

		bool IsReworked() const;

		void MakeNotTCKable();

	private:
		QString _brand, _speedAPIBrand;
		QString _type, _speedAPIType;
		tValue _topspeed;
		tValue _acceleration;
		tValue _handling;
		tValueF _nfswTopspeed[4];
		tValueF _nfswAcceleration[4];
		tValueF _nfswHandling[4];
		bool _hasNFSWPerformanceModel;
		tID _id;
		Acquirable _acquirable;
		tIGC _value;
		QStringList _images;
		bool _isReworked;
};

#endif /* __CCAR_H__ */
