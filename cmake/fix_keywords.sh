#!/bin/bash

sed -i -- 's/namespace public {/namespace public_ {/g' $1/*.pb.h $1/*.pb.cc
sed -i -- 's/tinkoff::public::/tinkoff::public_::/g' $1/*.pb.h $1/*.pb.cc
