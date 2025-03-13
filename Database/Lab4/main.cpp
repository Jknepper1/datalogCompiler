#include <iostream>
#include "Relation.h"
#include "Scheme.h"
#include "Tuple.h"

using namespace std;

int main() {
    Scheme scheme1( {"A", "B" } );
    Scheme scheme2( { "B", "C" } );

    Tuple tuple1( { "'1'", "'2'" } );
    Tuple tuple2( { "'3'", "'4'" } );

    Scheme scheme3( { "X", "Y" } );
    Scheme scheme4( { "X", "Y", "Z" } );

    Tuple tuple3( {"'1'", "'4'"} );
    Tuple tuple4( {"'1'", "'2'", "'4'"} );

    cout << Relation::joinable(scheme1, scheme2, tuple1, tuple2) << endl;
    // BC
    // 12
    // XY
    // 34
    cout << Relation::joinable(scheme2, scheme3, tuple1, tuple2) << endl;
    cout << Relation::joinable(scheme3, scheme4, tuple1, tuple4) << endl;
    cout << Relation::joinable(scheme3, scheme4, tuple3, tuple4) << endl;
    cout << Relation::joinable(scheme1, scheme2, tuple1, tuple2);
}