#!/bin/bash

sed -i -- 's/namespace public {/namespace p_ublic_ {/g' $1/*.pb.h $1/*.pb.cc
sed -i -- 's/tinkoff::public::/tinkoff::p_ublic_::/g' $1/*.pb.h $1/*.pb.cc
