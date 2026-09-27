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

#ifndef __TRDANALYZE_H__
#define __TRDANALYZE_H__

#include "Trd.h"

#include "../../Classes/TypeDefinitions.h"
#include <QVector>

class CBrand;
class CPart;
class MnuPartConstraints;

class TrdAnalyze: public Trd
{
	Q_OBJECT

	private:
		eCOMPARE Eval(const tPercent& _Topspeed, const tPercent& _Acceleration, const tPercent& _Handling);
		eCOMPARE Eval(const tPercent& _Topspeed, const tPercent& _Acceleration, const tPercent& _Handling,
				const CPart* _Engine, const CPart* _Induction, const CPart* _Transmission, const CPart* _Suspension,
				const CPart* _Brakes, const CPart* _Tires);

		void ComputeTargetPercents();
		bool CheckTargetPercents();

		struct AggregateTarget
		{
			int topspeed;
			int acceleration;
			int handling;
		};
		void BuildAggregateTargets();
		bool CanReachAggregate(int _Topspeed, int _Acceleration, int _Handling) const;

		void AnalyzeWithStock();
		void AnalyzeWithOutStock();
		void Analyze();

	public:
		TrdAnalyze(const QVector< CBrand* >* _Brands, const MnuPartConstraints* _PartConstraints, QObject* _Parent = 0);
		virtual ~TrdAnalyze();

		void SetTargetStats(const tValue& _Topsspeed, const tValue& _Acceleration, const tValue& _Handling);

		void run();

	private:
		tValue _topspeed;
		tValue _acceleration;
		tValue _handling;

		tPercent _pTopspeed;
		tPercent _pAcceleration;
		tPercent _pHandling;
		QVector< AggregateTarget > _aggregateTargets;
};

#endif /* __TRDANALYZE_H__ */
