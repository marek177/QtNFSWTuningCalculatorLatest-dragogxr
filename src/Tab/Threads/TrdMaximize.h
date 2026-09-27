/* **********************************************************************
 * NFS World - Tuning-Calculator                                        *
 * Copyright (C) 2011  dragoniac                                        *
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

#ifndef __TRDMAXIMIZE_H__
#define __TRDMAXIMIZE_H__

#include "Trd.h"

#include "../../Classes/EAttribute.h"
#include "../../Classes/TypeDefinitions.h"

class CBrand;
class CPart;
class MnuPartConstraints;

class TrdMaximize: public Trd
{
	Q_OBJECT

	private:
		void GetBestParts();

		eCOMPARE Eval(const tPercent& _Topspeed, const tPercent& _Acceleration, const tPercent& _Handling,
				const CPart* _Engine, const CPart* _Induction, const CPart* _Transmission, const CPart* _Suspension,
				const CPart* _Brakes, const CPart* _Tires);

		void Maximize();
		void MaximizeNFSWModel();

	public:
		TrdMaximize(const QVector< CBrand* >* _Brands, const MnuPartConstraints* _PartConstraints, QObject* _Parent = 0);
		virtual ~TrdMaximize();

		void SetMaximizeAttribute(const EAttribute& _Attribute);
		void SetConstraints(const CTuning* _Tuning);

		void run();

	public:
		static bool KeepPart(const CPart* _Part, const QVector< const CPart* >* _Parts);
		static void GetBestParts(QVector< const CPart* >* _Part, const CPart* _Constrained = 0);

	private:
		EAttribute _maximizedAttribute;
		tValue _currentMaximum;
		const CTuning* _constraint;
};

#endif /* __TRDMAXIMIZE_H__ */
