//this data structure holds the information passed by the ascii generator
//to the leaf injector "bodgelogger" contained in this package
//it is meant to hold event ID and beam information (which R3BRoot seems not
//to care about at the moment.

#ifndef R3B_ASCII_BLOG__H
#define R3B_ASCII_BLOG__H

#include "TObject.h"

//------------------------------------------------------------------------------------
//class declaration
class r3b_ascii_blog : public TObject {
	public:
		//core struct declaration
		struct entry {
			unsigned event_id;
			unsigned nb_tracks;
			float beam_momentum;
			float beam_impact_parameter;
			//TODO: more to come
		};
		
		r3b_ascii_blog() {};
		r3b_ascii_blog( const entry &given ):
			event_id( given.event_id ),
			nb_tracks( given.nb_tracks ),
			beam_momentum( given.beam_momentum ),
			beam_impact_parameter( given.beam_impact_parameter ) {};
		r3b_ascii_blog( const r3b_ascii_blog &given ):
			event_id( given.event_id ),
			nb_tracks( given.nb_tracks ),
			beam_momentum( given.beam_momentum ),
			beam_impact_parameter( given.beam_impact_parameter ) {};
		virtual ~r3b_ascii_blog() {};
		
		unsigned event_id;
		unsigned nb_tracks;
		float beam_momentum;
		float beam_impact_parameter;
		
		ClassDef( r3b_ascii_blog, 1 ); //hideous ROOT thing
};
typedef r3b_ascii_blog ascii_blog;

#endif
