/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2010 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/
#ifndef _MIRA_OPTIMISER_H_
#define _MIRA_OPTIMISER_H_

#include <vector>

#include "ScoreComponentCollection.h"


namespace Mira {
  
  class Optimiser {
    public:
      Optimiser() {}

      virtual size_t updateWeightsHopeFear(
      		Moses::ScoreComponentCollection& currWeights,
      		Moses::ScoreComponentCollection& weightUpdate,
				  const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesHope,
				  const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesFear,
				  const std::vector<std::vector<float> >& bleuScoresHope,
				  const std::vector<std::vector<float> >& bleuScoresFear,
				  const std::vector<std::vector<float> >& modelScoresHope,
				  const std::vector<std::vector<float> >& modelScoresFear,
				  float learning_rate,
				  size_t rank,
				  size_t epoch,
				  int updatePosition = -1) = 0;
  };
 
  class Perceptron : public Optimiser {
    public:
			virtual size_t updateWeightsHopeFear(
					Moses::ScoreComponentCollection& currWeights,
					Moses::ScoreComponentCollection& weightUpdate,
					const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesHope,
					const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesFear,
					const std::vector<std::vector<float> >& bleuScoresHope,
					const std::vector<std::vector<float> >& bleuScoresFear,
					const std::vector<std::vector<float> >& modelScoresHope,
					const std::vector<std::vector<float> >& modelScoresFear,
					float learning_rate,
					size_t rank,
					size_t epoch,
					int updatePosition = -1);
  };

  class MiraOptimiser : public Optimiser {
   public:
	  MiraOptimiser() :
		  Optimiser() { }

	  MiraOptimiser(bool onlyViolatedConstraints, float slack, size_t scale_margin,
size_t scale_update, float margin_slack, bool boost, size_t update_scheme, bool normaliseMargin) :
		  Optimiser(),
		  m_onlyViolatedConstraints(onlyViolatedConstraints),
		  m_slack(slack),
		  m_scale_margin(scale_margin),
		  m_scale_update(scale_update),
		  m_margin_slack(margin_slack),
		  m_boost(boost),
		  m_update_scheme(update_scheme),
		  m_normaliseMargin(normaliseMargin) { }
   
	  size_t updateWeights(Moses::ScoreComponentCollection& currWeights,
	  								Moses::ScoreComponentCollection& weightUpdate,
      						  const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValues,
      						  const std::vector<std::vector<float> >& losses,
      						  const std::vector<std::vector<float> >& bleuScores,
      						  const std::vector<std::vector<float> >& modelScores,
      						  const std::vector< Moses::ScoreComponentCollection>& oracleFeatureValues,
      						  const std::vector< float> oracleBleuScores,
      						  const std::vector< float> oracleModelScores,
      						  float learning_rate,
      						  size_t rank,
      						  size_t epoch);
     virtual size_t updateWeightsHopeFear(Moses::ScoreComponentCollection& currWeights,
    		 	 	 	 	 	 	Moses::ScoreComponentCollection& weightUpdate,
      						  const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesHope,
      						  const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValuesFear,
      						  const std::vector<std::vector<float> >& bleuScoresHope,
      						  const std::vector<std::vector<float> >& bleuScoresFear,
      						  const std::vector<std::vector<float> >& modelScoresHope,
      						  const std::vector<std::vector<float> >& modelScoresFear,
      						  float learning_rate,
      						  size_t rank,
      						  size_t epoch,
      						  int updatePosition = -1);
     size_t updateWeightsAnalytically(Moses::ScoreComponentCollection& currWeights,
    		 Moses::ScoreComponentCollection& weightUpdate,
    		 Moses::ScoreComponentCollection& featureValuesHope,
    		 Moses::ScoreComponentCollection& featureValuesFear,
    		 float bleuScoreHope,
    		 float bleuScoreFear,
    		 float modelScoreHope,
    		 float modelScoreFear,
    		 float learning_rate,
    		 size_t rank,
    		 size_t epoch);
     size_t updateWeightsRankModel(Moses::ScoreComponentCollection& currWeights,
    		 Moses::ScoreComponentCollection& weightUpdate,
    		 const std::vector<std::vector<Moses::ScoreComponentCollection> >& featureValues,
    		 const std::vector<std::vector<float> >& bleuScores,
    		 const std::vector<std::vector<float> >& modelScores,
    		 float learning_rate,
    		 size_t rank,
    		 size_t epoch);
     size_t updateWeightsHopeFearAndRankModel(
     		Moses::ScoreComponentCollection& currWeights,
     		Moses::ScoreComponentCollection& weightUpdate,
     		const std::vector< std::vector<Moses::ScoreComponentCollection> >& featureValuesHope,
     		const std::vector< std::vector<Moses::ScoreComponentCollection> >& featureValuesFear,
     		const std::vector< std::vector<Moses::ScoreComponentCollection> >& featureValues,
     		const std::vector<std::vector<float> >& bleuScoresHope,
     		const std::vector<std::vector<float> >& bleuScoresFear,
     		const std::vector<std::vector<float> >& bleuScores,
     		const std::vector<std::vector<float> >& modelScoresHope,
     		const std::vector<std::vector<float> >& modelScoresFear,
     		const std::vector<std::vector<float> >& modelScores,
     		float learning_rate,
     		size_t rank,
     		size_t epoch);

     void setSlack(float slack) {
    	 m_slack = slack;
     }

     void setMarginSlack(float margin_slack) {
    	 m_margin_slack = margin_slack;
     }

   private:

      // add only violated constraints to the optimisation problem
      bool m_onlyViolatedConstraints;

      // regularise Hildreth updates
      float m_slack;

      // slack when comparing losses to model scores
      float m_margin_slack;

      size_t m_scale_margin;

      // scale update with log 10 of oracle BLEU score
      size_t m_scale_update;

      // boosting of updates on misranked candidates
      bool m_boost;

      // select 1 of 5 different update schemes
      size_t m_update_scheme;

      // squash margin between 0 and 1
      bool m_normaliseMargin;
  };
}

#endif
