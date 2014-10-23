//
//  prinfo.h
//  hw2
//
//  Created by Yuan Yao on 10/22/14.
//  Copyright (c) 2014 Yuan Yao. All rights reserved.
//

#ifndef prinfo_h
#define prinfo_h
struct prinfo{
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};


#endif
