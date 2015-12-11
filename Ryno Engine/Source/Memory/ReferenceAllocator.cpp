//#include "ReferenceAllocator.h"
//
//namespace Ryno{
//
//	ReferenceAllocator* ReferenceAllocator::get_instance(){
//
//		static ReferenceAllocator instance;//only at the beginning
//		return &instance;
//	}
//
//	bool ReferenceAllocator::init(){
//	
//		return true;
//	}
//
//	void ReferenceAllocator::reset()
//	{
//		for (auto it : entries){
//			if (it.second == true){
//				free(it.first);
//			}
//		}
//	}
//
//}
