// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__MoDSS

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "./MoDSSCorrelParameter.h"
#include "./MoDSSUnpackProc.h"
#include "./MoDSSCorrelFact.h"
#include "./MoDSSAnlProc.h"
#include "./MoDSSCorrelEvent.h"
#include "./MoDSSAnalysis.h"
#include "./MoDSSSingleEvent.h"
#include "./MoDSSUnpackFact.h"
#include "./MoDSSCorrelProc.h"
#include "./MoDSSGlobals.h"
#include "./MoDSSHardwareParameter.h"
#include "./MoDSSAnlFact.h"
#include "./MoDSSDSSDEvent.h"
#include "./MoDSSPrintEventParameter.h"
#include "./MoDSSAnlEvent.h"
#include "./MoDSSCalibParameter.h"
#include "./MoDSSParameter.h"
#include "./MoDSSFilterParameter.h"
#include "./MoDSSUnpackEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_MoDSSAnalysis(void *p = 0);
   static void *newArray_MoDSSAnalysis(Long_t size, void *p);
   static void delete_MoDSSAnalysis(void *p);
   static void deleteArray_MoDSSAnalysis(void *p);
   static void destruct_MoDSSAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSAnalysis*)
   {
      ::MoDSSAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSAnalysis >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSAnalysis", ::MoDSSAnalysis::Class_Version(), "MoDSSAnalysis.h", 24,
                  typeid(::MoDSSAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSAnalysis) );
      instance.SetNew(&new_MoDSSAnalysis);
      instance.SetNewArray(&newArray_MoDSSAnalysis);
      instance.SetDelete(&delete_MoDSSAnalysis);
      instance.SetDeleteArray(&deleteArray_MoDSSAnalysis);
      instance.SetDestructor(&destruct_MoDSSAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSAnalysis*)
   {
      return GenerateInitInstanceLocal((::MoDSSAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSAnalysis*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSFilterParameter(void *p = 0);
   static void *newArray_MoDSSFilterParameter(Long_t size, void *p);
   static void delete_MoDSSFilterParameter(void *p);
   static void deleteArray_MoDSSFilterParameter(void *p);
   static void destruct_MoDSSFilterParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSFilterParameter*)
   {
      ::MoDSSFilterParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSFilterParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSFilterParameter", ::MoDSSFilterParameter::Class_Version(), "MoDSSFilterParameter.h", 18,
                  typeid(::MoDSSFilterParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSFilterParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSFilterParameter) );
      instance.SetNew(&new_MoDSSFilterParameter);
      instance.SetNewArray(&newArray_MoDSSFilterParameter);
      instance.SetDelete(&delete_MoDSSFilterParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSFilterParameter);
      instance.SetDestructor(&destruct_MoDSSFilterParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSFilterParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSFilterParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSFilterParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSUnpackProc(void *p = 0);
   static void *newArray_MoDSSUnpackProc(Long_t size, void *p);
   static void delete_MoDSSUnpackProc(void *p);
   static void deleteArray_MoDSSUnpackProc(void *p);
   static void destruct_MoDSSUnpackProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSUnpackProc*)
   {
      ::MoDSSUnpackProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSUnpackProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSUnpackProc", ::MoDSSUnpackProc::Class_Version(), "MoDSSUnpackProc.h", 88,
                  typeid(::MoDSSUnpackProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSUnpackProc::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSUnpackProc) );
      instance.SetNew(&new_MoDSSUnpackProc);
      instance.SetNewArray(&newArray_MoDSSUnpackProc);
      instance.SetDelete(&delete_MoDSSUnpackProc);
      instance.SetDeleteArray(&deleteArray_MoDSSUnpackProc);
      instance.SetDestructor(&destruct_MoDSSUnpackProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSUnpackProc*)
   {
      return GenerateInitInstanceLocal((::MoDSSUnpackProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSUnpackProc*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSUnpackEvent(void *p = 0);
   static void *newArray_MoDSSUnpackEvent(Long_t size, void *p);
   static void delete_MoDSSUnpackEvent(void *p);
   static void deleteArray_MoDSSUnpackEvent(void *p);
   static void destruct_MoDSSUnpackEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSUnpackEvent*)
   {
      ::MoDSSUnpackEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSUnpackEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSUnpackEvent", ::MoDSSUnpackEvent::Class_Version(), "MoDSSUnpackEvent.h", 27,
                  typeid(::MoDSSUnpackEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSUnpackEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSUnpackEvent) );
      instance.SetNew(&new_MoDSSUnpackEvent);
      instance.SetNewArray(&newArray_MoDSSUnpackEvent);
      instance.SetDelete(&delete_MoDSSUnpackEvent);
      instance.SetDeleteArray(&deleteArray_MoDSSUnpackEvent);
      instance.SetDestructor(&destruct_MoDSSUnpackEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSUnpackEvent*)
   {
      return GenerateInitInstanceLocal((::MoDSSUnpackEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSUnpackEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSUnpackFact(void *p = 0);
   static void *newArray_MoDSSUnpackFact(Long_t size, void *p);
   static void delete_MoDSSUnpackFact(void *p);
   static void deleteArray_MoDSSUnpackFact(void *p);
   static void destruct_MoDSSUnpackFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSUnpackFact*)
   {
      ::MoDSSUnpackFact *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSUnpackFact >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSUnpackFact", ::MoDSSUnpackFact::Class_Version(), "MoDSSUnpackFact.h", 15,
                  typeid(::MoDSSUnpackFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSUnpackFact::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSUnpackFact) );
      instance.SetNew(&new_MoDSSUnpackFact);
      instance.SetNewArray(&newArray_MoDSSUnpackFact);
      instance.SetDelete(&delete_MoDSSUnpackFact);
      instance.SetDeleteArray(&deleteArray_MoDSSUnpackFact);
      instance.SetDestructor(&destruct_MoDSSUnpackFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSUnpackFact*)
   {
      return GenerateInitInstanceLocal((::MoDSSUnpackFact*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSUnpackFact*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSCalibParameter(void *p = 0);
   static void *newArray_MoDSSCalibParameter(Long_t size, void *p);
   static void delete_MoDSSCalibParameter(void *p);
   static void deleteArray_MoDSSCalibParameter(void *p);
   static void destruct_MoDSSCalibParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSCalibParameter*)
   {
      ::MoDSSCalibParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSCalibParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSCalibParameter", ::MoDSSCalibParameter::Class_Version(), "MoDSSCalibParameter.h", 8,
                  typeid(::MoDSSCalibParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSCalibParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSCalibParameter) );
      instance.SetNew(&new_MoDSSCalibParameter);
      instance.SetNewArray(&newArray_MoDSSCalibParameter);
      instance.SetDelete(&delete_MoDSSCalibParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSCalibParameter);
      instance.SetDestructor(&destruct_MoDSSCalibParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSCalibParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSCalibParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSCalibParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSAnlProc(void *p = 0);
   static void *newArray_MoDSSAnlProc(Long_t size, void *p);
   static void delete_MoDSSAnlProc(void *p);
   static void deleteArray_MoDSSAnlProc(void *p);
   static void destruct_MoDSSAnlProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSAnlProc*)
   {
      ::MoDSSAnlProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSAnlProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSAnlProc", ::MoDSSAnlProc::Class_Version(), "MoDSSAnlProc.h", 42,
                  typeid(::MoDSSAnlProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSAnlProc::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSAnlProc) );
      instance.SetNew(&new_MoDSSAnlProc);
      instance.SetNewArray(&newArray_MoDSSAnlProc);
      instance.SetDelete(&delete_MoDSSAnlProc);
      instance.SetDeleteArray(&deleteArray_MoDSSAnlProc);
      instance.SetDestructor(&destruct_MoDSSAnlProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSAnlProc*)
   {
      return GenerateInitInstanceLocal((::MoDSSAnlProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSAnlProc*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSAnlEvent(void *p = 0);
   static void *newArray_MoDSSAnlEvent(Long_t size, void *p);
   static void delete_MoDSSAnlEvent(void *p);
   static void deleteArray_MoDSSAnlEvent(void *p);
   static void destruct_MoDSSAnlEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSAnlEvent*)
   {
      ::MoDSSAnlEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSAnlEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSAnlEvent", ::MoDSSAnlEvent::Class_Version(), "MoDSSAnlEvent.h", 12,
                  typeid(::MoDSSAnlEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSAnlEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSAnlEvent) );
      instance.SetNew(&new_MoDSSAnlEvent);
      instance.SetNewArray(&newArray_MoDSSAnlEvent);
      instance.SetDelete(&delete_MoDSSAnlEvent);
      instance.SetDeleteArray(&deleteArray_MoDSSAnlEvent);
      instance.SetDestructor(&destruct_MoDSSAnlEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSAnlEvent*)
   {
      return GenerateInitInstanceLocal((::MoDSSAnlEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSAnlEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSAnlFact(void *p = 0);
   static void *newArray_MoDSSAnlFact(Long_t size, void *p);
   static void delete_MoDSSAnlFact(void *p);
   static void deleteArray_MoDSSAnlFact(void *p);
   static void destruct_MoDSSAnlFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSAnlFact*)
   {
      ::MoDSSAnlFact *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSAnlFact >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSAnlFact", ::MoDSSAnlFact::Class_Version(), "MoDSSAnlFact.h", 11,
                  typeid(::MoDSSAnlFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSAnlFact::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSAnlFact) );
      instance.SetNew(&new_MoDSSAnlFact);
      instance.SetNewArray(&newArray_MoDSSAnlFact);
      instance.SetDelete(&delete_MoDSSAnlFact);
      instance.SetDeleteArray(&deleteArray_MoDSSAnlFact);
      instance.SetDestructor(&destruct_MoDSSAnlFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSAnlFact*)
   {
      return GenerateInitInstanceLocal((::MoDSSAnlFact*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSAnlFact*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSParameter(void *p = 0);
   static void *newArray_MoDSSParameter(Long_t size, void *p);
   static void delete_MoDSSParameter(void *p);
   static void deleteArray_MoDSSParameter(void *p);
   static void destruct_MoDSSParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSParameter*)
   {
      ::MoDSSParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSParameter", ::MoDSSParameter::Class_Version(), "MoDSSParameter.h", 12,
                  typeid(::MoDSSParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSParameter) );
      instance.SetNew(&new_MoDSSParameter);
      instance.SetNewArray(&newArray_MoDSSParameter);
      instance.SetDelete(&delete_MoDSSParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSParameter);
      instance.SetDestructor(&destruct_MoDSSParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSCorrelProc(void *p = 0);
   static void *newArray_MoDSSCorrelProc(Long_t size, void *p);
   static void delete_MoDSSCorrelProc(void *p);
   static void deleteArray_MoDSSCorrelProc(void *p);
   static void destruct_MoDSSCorrelProc(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSCorrelProc*)
   {
      ::MoDSSCorrelProc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSCorrelProc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSCorrelProc", ::MoDSSCorrelProc::Class_Version(), "MoDSSCorrelProc.h", 38,
                  typeid(::MoDSSCorrelProc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSCorrelProc::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSCorrelProc) );
      instance.SetNew(&new_MoDSSCorrelProc);
      instance.SetNewArray(&newArray_MoDSSCorrelProc);
      instance.SetDelete(&delete_MoDSSCorrelProc);
      instance.SetDeleteArray(&deleteArray_MoDSSCorrelProc);
      instance.SetDestructor(&destruct_MoDSSCorrelProc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSCorrelProc*)
   {
      return GenerateInitInstanceLocal((::MoDSSCorrelProc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSCorrelProc*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSCorrelEvent(void *p = 0);
   static void *newArray_MoDSSCorrelEvent(Long_t size, void *p);
   static void delete_MoDSSCorrelEvent(void *p);
   static void deleteArray_MoDSSCorrelEvent(void *p);
   static void destruct_MoDSSCorrelEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSCorrelEvent*)
   {
      ::MoDSSCorrelEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSCorrelEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSCorrelEvent", ::MoDSSCorrelEvent::Class_Version(), "MoDSSCorrelEvent.h", 10,
                  typeid(::MoDSSCorrelEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSCorrelEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSCorrelEvent) );
      instance.SetNew(&new_MoDSSCorrelEvent);
      instance.SetNewArray(&newArray_MoDSSCorrelEvent);
      instance.SetDelete(&delete_MoDSSCorrelEvent);
      instance.SetDeleteArray(&deleteArray_MoDSSCorrelEvent);
      instance.SetDestructor(&destruct_MoDSSCorrelEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSCorrelEvent*)
   {
      return GenerateInitInstanceLocal((::MoDSSCorrelEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSCorrelEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSCorrelFact(void *p = 0);
   static void *newArray_MoDSSCorrelFact(Long_t size, void *p);
   static void delete_MoDSSCorrelFact(void *p);
   static void deleteArray_MoDSSCorrelFact(void *p);
   static void destruct_MoDSSCorrelFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSCorrelFact*)
   {
      ::MoDSSCorrelFact *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSCorrelFact >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSCorrelFact", ::MoDSSCorrelFact::Class_Version(), "MoDSSCorrelFact.h", 12,
                  typeid(::MoDSSCorrelFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSCorrelFact::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSCorrelFact) );
      instance.SetNew(&new_MoDSSCorrelFact);
      instance.SetNewArray(&newArray_MoDSSCorrelFact);
      instance.SetDelete(&delete_MoDSSCorrelFact);
      instance.SetDeleteArray(&deleteArray_MoDSSCorrelFact);
      instance.SetDestructor(&destruct_MoDSSCorrelFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSCorrelFact*)
   {
      return GenerateInitInstanceLocal((::MoDSSCorrelFact*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSCorrelFact*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSCorrelParameter(void *p = 0);
   static void *newArray_MoDSSCorrelParameter(Long_t size, void *p);
   static void delete_MoDSSCorrelParameter(void *p);
   static void deleteArray_MoDSSCorrelParameter(void *p);
   static void destruct_MoDSSCorrelParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSCorrelParameter*)
   {
      ::MoDSSCorrelParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSCorrelParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSCorrelParameter", ::MoDSSCorrelParameter::Class_Version(), "MoDSSCorrelParameter.h", 13,
                  typeid(::MoDSSCorrelParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSCorrelParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSCorrelParameter) );
      instance.SetNew(&new_MoDSSCorrelParameter);
      instance.SetNewArray(&newArray_MoDSSCorrelParameter);
      instance.SetDelete(&delete_MoDSSCorrelParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSCorrelParameter);
      instance.SetDestructor(&destruct_MoDSSCorrelParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSCorrelParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSCorrelParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSCorrelParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSDSSDEvent(void *p = 0);
   static void *newArray_MoDSSDSSDEvent(Long_t size, void *p);
   static void delete_MoDSSDSSDEvent(void *p);
   static void deleteArray_MoDSSDSSDEvent(void *p);
   static void destruct_MoDSSDSSDEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSDSSDEvent*)
   {
      ::MoDSSDSSDEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSDSSDEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSDSSDEvent", ::MoDSSDSSDEvent::Class_Version(), "MoDSSDSSDEvent.h", 16,
                  typeid(::MoDSSDSSDEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSDSSDEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSDSSDEvent) );
      instance.SetNew(&new_MoDSSDSSDEvent);
      instance.SetNewArray(&newArray_MoDSSDSSDEvent);
      instance.SetDelete(&delete_MoDSSDSSDEvent);
      instance.SetDeleteArray(&deleteArray_MoDSSDSSDEvent);
      instance.SetDestructor(&destruct_MoDSSDSSDEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSDSSDEvent*)
   {
      return GenerateInitInstanceLocal((::MoDSSDSSDEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSDSSDEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSSingleEvent(void *p = 0);
   static void *newArray_MoDSSSingleEvent(Long_t size, void *p);
   static void delete_MoDSSSingleEvent(void *p);
   static void deleteArray_MoDSSSingleEvent(void *p);
   static void destruct_MoDSSSingleEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSSingleEvent*)
   {
      ::MoDSSSingleEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSSingleEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSSingleEvent", ::MoDSSSingleEvent::Class_Version(), "MoDSSSingleEvent.h", 12,
                  typeid(::MoDSSSingleEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSSingleEvent::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSSingleEvent) );
      instance.SetNew(&new_MoDSSSingleEvent);
      instance.SetNewArray(&newArray_MoDSSSingleEvent);
      instance.SetDelete(&delete_MoDSSSingleEvent);
      instance.SetDeleteArray(&deleteArray_MoDSSSingleEvent);
      instance.SetDestructor(&destruct_MoDSSSingleEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSSingleEvent*)
   {
      return GenerateInitInstanceLocal((::MoDSSSingleEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSSingleEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSHardwareParameter(void *p = 0);
   static void *newArray_MoDSSHardwareParameter(Long_t size, void *p);
   static void delete_MoDSSHardwareParameter(void *p);
   static void deleteArray_MoDSSHardwareParameter(void *p);
   static void destruct_MoDSSHardwareParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSHardwareParameter*)
   {
      ::MoDSSHardwareParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSHardwareParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSHardwareParameter", ::MoDSSHardwareParameter::Class_Version(), "MoDSSHardwareParameter.h", 13,
                  typeid(::MoDSSHardwareParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSHardwareParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSHardwareParameter) );
      instance.SetNew(&new_MoDSSHardwareParameter);
      instance.SetNewArray(&newArray_MoDSSHardwareParameter);
      instance.SetDelete(&delete_MoDSSHardwareParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSHardwareParameter);
      instance.SetDestructor(&destruct_MoDSSHardwareParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSHardwareParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSHardwareParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSHardwareParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_MoDSSPrintEventParameter(void *p = 0);
   static void *newArray_MoDSSPrintEventParameter(Long_t size, void *p);
   static void delete_MoDSSPrintEventParameter(void *p);
   static void deleteArray_MoDSSPrintEventParameter(void *p);
   static void destruct_MoDSSPrintEventParameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MoDSSPrintEventParameter*)
   {
      ::MoDSSPrintEventParameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MoDSSPrintEventParameter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("MoDSSPrintEventParameter", ::MoDSSPrintEventParameter::Class_Version(), "MoDSSPrintEventParameter.h", 12,
                  typeid(::MoDSSPrintEventParameter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MoDSSPrintEventParameter::Dictionary, isa_proxy, 4,
                  sizeof(::MoDSSPrintEventParameter) );
      instance.SetNew(&new_MoDSSPrintEventParameter);
      instance.SetNewArray(&newArray_MoDSSPrintEventParameter);
      instance.SetDelete(&delete_MoDSSPrintEventParameter);
      instance.SetDeleteArray(&deleteArray_MoDSSPrintEventParameter);
      instance.SetDestructor(&destruct_MoDSSPrintEventParameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MoDSSPrintEventParameter*)
   {
      return GenerateInitInstanceLocal((::MoDSSPrintEventParameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MoDSSPrintEventParameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MoDSSAnalysis::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSAnalysis::Class_Name()
{
   return "MoDSSAnalysis";
}

//______________________________________________________________________________
const char *MoDSSAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnalysis*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnalysis*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnalysis*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnalysis*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSFilterParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSFilterParameter::Class_Name()
{
   return "MoDSSFilterParameter";
}

//______________________________________________________________________________
const char *MoDSSFilterParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSFilterParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSFilterParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSFilterParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSFilterParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSFilterParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSFilterParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSFilterParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSUnpackProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSUnpackProc::Class_Name()
{
   return "MoDSSUnpackProc";
}

//______________________________________________________________________________
const char *MoDSSUnpackProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSUnpackProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSUnpackProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSUnpackProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSUnpackEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSUnpackEvent::Class_Name()
{
   return "MoDSSUnpackEvent";
}

//______________________________________________________________________________
const char *MoDSSUnpackEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSUnpackEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSUnpackEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSUnpackEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSUnpackFact::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSUnpackFact::Class_Name()
{
   return "MoDSSUnpackFact";
}

//______________________________________________________________________________
const char *MoDSSUnpackFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackFact*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSUnpackFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackFact*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSUnpackFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackFact*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSUnpackFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSUnpackFact*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSCalibParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSCalibParameter::Class_Name()
{
   return "MoDSSCalibParameter";
}

//______________________________________________________________________________
const char *MoDSSCalibParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCalibParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSCalibParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCalibParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSCalibParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCalibParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSCalibParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCalibParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSAnlProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSAnlProc::Class_Name()
{
   return "MoDSSAnlProc";
}

//______________________________________________________________________________
const char *MoDSSAnlProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSAnlProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSAnlProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSAnlProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSAnlEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSAnlEvent::Class_Name()
{
   return "MoDSSAnlEvent";
}

//______________________________________________________________________________
const char *MoDSSAnlEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSAnlEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSAnlEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSAnlEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSAnlFact::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSAnlFact::Class_Name()
{
   return "MoDSSAnlFact";
}

//______________________________________________________________________________
const char *MoDSSAnlFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlFact*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSAnlFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlFact*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSAnlFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlFact*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSAnlFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSAnlFact*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSParameter::Class_Name()
{
   return "MoDSSParameter";
}

//______________________________________________________________________________
const char *MoDSSParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSCorrelProc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSCorrelProc::Class_Name()
{
   return "MoDSSCorrelProc";
}

//______________________________________________________________________________
const char *MoDSSCorrelProc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelProc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSCorrelProc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelProc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSCorrelProc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelProc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSCorrelProc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelProc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSCorrelEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSCorrelEvent::Class_Name()
{
   return "MoDSSCorrelEvent";
}

//______________________________________________________________________________
const char *MoDSSCorrelEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSCorrelEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSCorrelEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSCorrelEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSCorrelFact::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSCorrelFact::Class_Name()
{
   return "MoDSSCorrelFact";
}

//______________________________________________________________________________
const char *MoDSSCorrelFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelFact*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSCorrelFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelFact*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSCorrelFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelFact*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSCorrelFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelFact*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSCorrelParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSCorrelParameter::Class_Name()
{
   return "MoDSSCorrelParameter";
}

//______________________________________________________________________________
const char *MoDSSCorrelParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSCorrelParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSCorrelParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSCorrelParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSCorrelParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSDSSDEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSDSSDEvent::Class_Name()
{
   return "MoDSSDSSDEvent";
}

//______________________________________________________________________________
const char *MoDSSDSSDEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSDSSDEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSDSSDEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSDSSDEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSDSSDEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSDSSDEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSDSSDEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSDSSDEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSSingleEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSSingleEvent::Class_Name()
{
   return "MoDSSSingleEvent";
}

//______________________________________________________________________________
const char *MoDSSSingleEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSSingleEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSSingleEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSSingleEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSSingleEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSSingleEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSSingleEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSSingleEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSHardwareParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSHardwareParameter::Class_Name()
{
   return "MoDSSHardwareParameter";
}

//______________________________________________________________________________
const char *MoDSSHardwareParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSHardwareParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSHardwareParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSHardwareParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSHardwareParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSHardwareParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSHardwareParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSHardwareParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr MoDSSPrintEventParameter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *MoDSSPrintEventParameter::Class_Name()
{
   return "MoDSSPrintEventParameter";
}

//______________________________________________________________________________
const char *MoDSSPrintEventParameter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSPrintEventParameter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int MoDSSPrintEventParameter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MoDSSPrintEventParameter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MoDSSPrintEventParameter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSPrintEventParameter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MoDSSPrintEventParameter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MoDSSPrintEventParameter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MoDSSAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSAnalysis(void *p) {
      return  p ? new(p) ::MoDSSAnalysis : new ::MoDSSAnalysis;
   }
   static void *newArray_MoDSSAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSAnalysis[nElements] : new ::MoDSSAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSAnalysis(void *p) {
      delete ((::MoDSSAnalysis*)p);
   }
   static void deleteArray_MoDSSAnalysis(void *p) {
      delete [] ((::MoDSSAnalysis*)p);
   }
   static void destruct_MoDSSAnalysis(void *p) {
      typedef ::MoDSSAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSAnalysis

//______________________________________________________________________________
void MoDSSFilterParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSFilterParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSFilterParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSFilterParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSFilterParameter(void *p) {
      return  p ? new(p) ::MoDSSFilterParameter : new ::MoDSSFilterParameter;
   }
   static void *newArray_MoDSSFilterParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSFilterParameter[nElements] : new ::MoDSSFilterParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSFilterParameter(void *p) {
      delete ((::MoDSSFilterParameter*)p);
   }
   static void deleteArray_MoDSSFilterParameter(void *p) {
      delete [] ((::MoDSSFilterParameter*)p);
   }
   static void destruct_MoDSSFilterParameter(void *p) {
      typedef ::MoDSSFilterParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSFilterParameter

//______________________________________________________________________________
void MoDSSUnpackProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSUnpackProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSUnpackProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSUnpackProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSUnpackProc(void *p) {
      return  p ? new(p) ::MoDSSUnpackProc : new ::MoDSSUnpackProc;
   }
   static void *newArray_MoDSSUnpackProc(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSUnpackProc[nElements] : new ::MoDSSUnpackProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSUnpackProc(void *p) {
      delete ((::MoDSSUnpackProc*)p);
   }
   static void deleteArray_MoDSSUnpackProc(void *p) {
      delete [] ((::MoDSSUnpackProc*)p);
   }
   static void destruct_MoDSSUnpackProc(void *p) {
      typedef ::MoDSSUnpackProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSUnpackProc

//______________________________________________________________________________
void MoDSSUnpackEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSUnpackEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSUnpackEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSUnpackEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSUnpackEvent(void *p) {
      return  p ? new(p) ::MoDSSUnpackEvent : new ::MoDSSUnpackEvent;
   }
   static void *newArray_MoDSSUnpackEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSUnpackEvent[nElements] : new ::MoDSSUnpackEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSUnpackEvent(void *p) {
      delete ((::MoDSSUnpackEvent*)p);
   }
   static void deleteArray_MoDSSUnpackEvent(void *p) {
      delete [] ((::MoDSSUnpackEvent*)p);
   }
   static void destruct_MoDSSUnpackEvent(void *p) {
      typedef ::MoDSSUnpackEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSUnpackEvent

//______________________________________________________________________________
void MoDSSUnpackFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSUnpackFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSUnpackFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSUnpackFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSUnpackFact(void *p) {
      return  p ? new(p) ::MoDSSUnpackFact : new ::MoDSSUnpackFact;
   }
   static void *newArray_MoDSSUnpackFact(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSUnpackFact[nElements] : new ::MoDSSUnpackFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSUnpackFact(void *p) {
      delete ((::MoDSSUnpackFact*)p);
   }
   static void deleteArray_MoDSSUnpackFact(void *p) {
      delete [] ((::MoDSSUnpackFact*)p);
   }
   static void destruct_MoDSSUnpackFact(void *p) {
      typedef ::MoDSSUnpackFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSUnpackFact

//______________________________________________________________________________
void MoDSSCalibParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSCalibParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSCalibParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSCalibParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSCalibParameter(void *p) {
      return  p ? new(p) ::MoDSSCalibParameter : new ::MoDSSCalibParameter;
   }
   static void *newArray_MoDSSCalibParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSCalibParameter[nElements] : new ::MoDSSCalibParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSCalibParameter(void *p) {
      delete ((::MoDSSCalibParameter*)p);
   }
   static void deleteArray_MoDSSCalibParameter(void *p) {
      delete [] ((::MoDSSCalibParameter*)p);
   }
   static void destruct_MoDSSCalibParameter(void *p) {
      typedef ::MoDSSCalibParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSCalibParameter

//______________________________________________________________________________
void MoDSSAnlProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSAnlProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSAnlProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSAnlProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSAnlProc(void *p) {
      return  p ? new(p) ::MoDSSAnlProc : new ::MoDSSAnlProc;
   }
   static void *newArray_MoDSSAnlProc(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSAnlProc[nElements] : new ::MoDSSAnlProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSAnlProc(void *p) {
      delete ((::MoDSSAnlProc*)p);
   }
   static void deleteArray_MoDSSAnlProc(void *p) {
      delete [] ((::MoDSSAnlProc*)p);
   }
   static void destruct_MoDSSAnlProc(void *p) {
      typedef ::MoDSSAnlProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSAnlProc

//______________________________________________________________________________
void MoDSSAnlEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSAnlEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSAnlEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSAnlEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSAnlEvent(void *p) {
      return  p ? new(p) ::MoDSSAnlEvent : new ::MoDSSAnlEvent;
   }
   static void *newArray_MoDSSAnlEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSAnlEvent[nElements] : new ::MoDSSAnlEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSAnlEvent(void *p) {
      delete ((::MoDSSAnlEvent*)p);
   }
   static void deleteArray_MoDSSAnlEvent(void *p) {
      delete [] ((::MoDSSAnlEvent*)p);
   }
   static void destruct_MoDSSAnlEvent(void *p) {
      typedef ::MoDSSAnlEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSAnlEvent

//______________________________________________________________________________
void MoDSSAnlFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSAnlFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSAnlFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSAnlFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSAnlFact(void *p) {
      return  p ? new(p) ::MoDSSAnlFact : new ::MoDSSAnlFact;
   }
   static void *newArray_MoDSSAnlFact(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSAnlFact[nElements] : new ::MoDSSAnlFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSAnlFact(void *p) {
      delete ((::MoDSSAnlFact*)p);
   }
   static void deleteArray_MoDSSAnlFact(void *p) {
      delete [] ((::MoDSSAnlFact*)p);
   }
   static void destruct_MoDSSAnlFact(void *p) {
      typedef ::MoDSSAnlFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSAnlFact

//______________________________________________________________________________
void MoDSSParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSParameter(void *p) {
      return  p ? new(p) ::MoDSSParameter : new ::MoDSSParameter;
   }
   static void *newArray_MoDSSParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSParameter[nElements] : new ::MoDSSParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSParameter(void *p) {
      delete ((::MoDSSParameter*)p);
   }
   static void deleteArray_MoDSSParameter(void *p) {
      delete [] ((::MoDSSParameter*)p);
   }
   static void destruct_MoDSSParameter(void *p) {
      typedef ::MoDSSParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSParameter

//______________________________________________________________________________
void MoDSSCorrelProc::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSCorrelProc.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSCorrelProc::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSCorrelProc::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSCorrelProc(void *p) {
      return  p ? new(p) ::MoDSSCorrelProc : new ::MoDSSCorrelProc;
   }
   static void *newArray_MoDSSCorrelProc(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSCorrelProc[nElements] : new ::MoDSSCorrelProc[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSCorrelProc(void *p) {
      delete ((::MoDSSCorrelProc*)p);
   }
   static void deleteArray_MoDSSCorrelProc(void *p) {
      delete [] ((::MoDSSCorrelProc*)p);
   }
   static void destruct_MoDSSCorrelProc(void *p) {
      typedef ::MoDSSCorrelProc current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSCorrelProc

//______________________________________________________________________________
void MoDSSCorrelEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSCorrelEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSCorrelEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSCorrelEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSCorrelEvent(void *p) {
      return  p ? new(p) ::MoDSSCorrelEvent : new ::MoDSSCorrelEvent;
   }
   static void *newArray_MoDSSCorrelEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSCorrelEvent[nElements] : new ::MoDSSCorrelEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSCorrelEvent(void *p) {
      delete ((::MoDSSCorrelEvent*)p);
   }
   static void deleteArray_MoDSSCorrelEvent(void *p) {
      delete [] ((::MoDSSCorrelEvent*)p);
   }
   static void destruct_MoDSSCorrelEvent(void *p) {
      typedef ::MoDSSCorrelEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSCorrelEvent

//______________________________________________________________________________
void MoDSSCorrelFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSCorrelFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSCorrelFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSCorrelFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSCorrelFact(void *p) {
      return  p ? new(p) ::MoDSSCorrelFact : new ::MoDSSCorrelFact;
   }
   static void *newArray_MoDSSCorrelFact(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSCorrelFact[nElements] : new ::MoDSSCorrelFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSCorrelFact(void *p) {
      delete ((::MoDSSCorrelFact*)p);
   }
   static void deleteArray_MoDSSCorrelFact(void *p) {
      delete [] ((::MoDSSCorrelFact*)p);
   }
   static void destruct_MoDSSCorrelFact(void *p) {
      typedef ::MoDSSCorrelFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSCorrelFact

//______________________________________________________________________________
void MoDSSCorrelParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSCorrelParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSCorrelParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSCorrelParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSCorrelParameter(void *p) {
      return  p ? new(p) ::MoDSSCorrelParameter : new ::MoDSSCorrelParameter;
   }
   static void *newArray_MoDSSCorrelParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSCorrelParameter[nElements] : new ::MoDSSCorrelParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSCorrelParameter(void *p) {
      delete ((::MoDSSCorrelParameter*)p);
   }
   static void deleteArray_MoDSSCorrelParameter(void *p) {
      delete [] ((::MoDSSCorrelParameter*)p);
   }
   static void destruct_MoDSSCorrelParameter(void *p) {
      typedef ::MoDSSCorrelParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSCorrelParameter

//______________________________________________________________________________
void MoDSSDSSDEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSDSSDEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSDSSDEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSDSSDEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSDSSDEvent(void *p) {
      return  p ? new(p) ::MoDSSDSSDEvent : new ::MoDSSDSSDEvent;
   }
   static void *newArray_MoDSSDSSDEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSDSSDEvent[nElements] : new ::MoDSSDSSDEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSDSSDEvent(void *p) {
      delete ((::MoDSSDSSDEvent*)p);
   }
   static void deleteArray_MoDSSDSSDEvent(void *p) {
      delete [] ((::MoDSSDSSDEvent*)p);
   }
   static void destruct_MoDSSDSSDEvent(void *p) {
      typedef ::MoDSSDSSDEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSDSSDEvent

//______________________________________________________________________________
void MoDSSSingleEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSSingleEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSSingleEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSSingleEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSSingleEvent(void *p) {
      return  p ? new(p) ::MoDSSSingleEvent : new ::MoDSSSingleEvent;
   }
   static void *newArray_MoDSSSingleEvent(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSSingleEvent[nElements] : new ::MoDSSSingleEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSSingleEvent(void *p) {
      delete ((::MoDSSSingleEvent*)p);
   }
   static void deleteArray_MoDSSSingleEvent(void *p) {
      delete [] ((::MoDSSSingleEvent*)p);
   }
   static void destruct_MoDSSSingleEvent(void *p) {
      typedef ::MoDSSSingleEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSSingleEvent

//______________________________________________________________________________
void MoDSSHardwareParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSHardwareParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSHardwareParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSHardwareParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSHardwareParameter(void *p) {
      return  p ? new(p) ::MoDSSHardwareParameter : new ::MoDSSHardwareParameter;
   }
   static void *newArray_MoDSSHardwareParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSHardwareParameter[nElements] : new ::MoDSSHardwareParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSHardwareParameter(void *p) {
      delete ((::MoDSSHardwareParameter*)p);
   }
   static void deleteArray_MoDSSHardwareParameter(void *p) {
      delete [] ((::MoDSSHardwareParameter*)p);
   }
   static void destruct_MoDSSHardwareParameter(void *p) {
      typedef ::MoDSSHardwareParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSHardwareParameter

//______________________________________________________________________________
void MoDSSPrintEventParameter::Streamer(TBuffer &R__b)
{
   // Stream an object of class MoDSSPrintEventParameter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(MoDSSPrintEventParameter::Class(),this);
   } else {
      R__b.WriteClassBuffer(MoDSSPrintEventParameter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_MoDSSPrintEventParameter(void *p) {
      return  p ? new(p) ::MoDSSPrintEventParameter : new ::MoDSSPrintEventParameter;
   }
   static void *newArray_MoDSSPrintEventParameter(Long_t nElements, void *p) {
      return p ? new(p) ::MoDSSPrintEventParameter[nElements] : new ::MoDSSPrintEventParameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_MoDSSPrintEventParameter(void *p) {
      delete ((::MoDSSPrintEventParameter*)p);
   }
   static void deleteArray_MoDSSPrintEventParameter(void *p) {
      delete [] ((::MoDSSPrintEventParameter*)p);
   }
   static void destruct_MoDSSPrintEventParameter(void *p) {
      typedef ::MoDSSPrintEventParameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MoDSSPrintEventParameter

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 210,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 210,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlELong64_tgR_Dictionary();
   static void vectorlELong64_tgR_TClassManip(TClass*);
   static void *new_vectorlELong64_tgR(void *p = 0);
   static void *newArray_vectorlELong64_tgR(Long_t size, void *p);
   static void delete_vectorlELong64_tgR(void *p);
   static void deleteArray_vectorlELong64_tgR(void *p);
   static void destruct_vectorlELong64_tgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Long64_t>*)
   {
      vector<Long64_t> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Long64_t>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Long64_t>", -2, "vector", 210,
                  typeid(vector<Long64_t>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlELong64_tgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<Long64_t>) );
      instance.SetNew(&new_vectorlELong64_tgR);
      instance.SetNewArray(&newArray_vectorlELong64_tgR);
      instance.SetDelete(&delete_vectorlELong64_tgR);
      instance.SetDeleteArray(&deleteArray_vectorlELong64_tgR);
      instance.SetDestructor(&destruct_vectorlELong64_tgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Long64_t> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<Long64_t>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlELong64_tgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Long64_t>*)0x0)->GetClass();
      vectorlELong64_tgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlELong64_tgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlELong64_tgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Long64_t> : new vector<Long64_t>;
   }
   static void *newArray_vectorlELong64_tgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Long64_t>[nElements] : new vector<Long64_t>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlELong64_tgR(void *p) {
      delete ((vector<Long64_t>*)p);
   }
   static void deleteArray_vectorlELong64_tgR(void *p) {
      delete [] ((vector<Long64_t>*)p);
   }
   static void destruct_vectorlELong64_tgR(void *p) {
      typedef vector<Long64_t> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Long64_t>

namespace {
  void TriggerDictionaryInitialization_libGo4UserAnalysis_Impl() {
    static const char* headers[] = {
"./MoDSSCorrelParameter.h",
"./MoDSSUnpackProc.h",
"./MoDSSCorrelFact.h",
"./MoDSSAnlProc.h",
"./MoDSSCorrelEvent.h",
"./MoDSSAnalysis.h",
"./MoDSSSingleEvent.h",
"./MoDSSUnpackFact.h",
"./MoDSSCorrelProc.h",
"./MoDSSGlobals.h",
"./MoDSSHardwareParameter.h",
"./MoDSSAnlFact.h",
"./MoDSSDSSDEvent.h",
"./MoDSSPrintEventParameter.h",
"./MoDSSAnlEvent.h",
"./MoDSSCalibParameter.h",
"./MoDSSParameter.h",
"./MoDSSFilterParameter.h",
"./MoDSSUnpackEvent.h",
0
    };
    static const char* includePaths[] = {
"/opt/go4-5.1.2-qt4/include",
"/opt/go4-5.1.2-qt4",
"/opt/root_v6.06.06/include",
"/home/e656/E686_Analyse/localcopy/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libGo4UserAnalysis dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$./MoDSSAnalysis.h")))  MoDSSAnalysis;
class __attribute__((annotate("$clingAutoload$./MoDSSUnpackProc.h")))  MoDSSFilterParameter;
class __attribute__((annotate("$clingAutoload$./MoDSSUnpackProc.h")))  MoDSSUnpackProc;
class __attribute__((annotate("$clingAutoload$./MoDSSUnpackProc.h")))  MoDSSUnpackEvent;
class __attribute__((annotate("$clingAutoload$./MoDSSAnalysis.h")))  MoDSSUnpackFact;
class __attribute__((annotate("$clingAutoload$./MoDSSUnpackProc.h")))  MoDSSCalibParameter;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSAnlProc;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSAnlEvent;
class __attribute__((annotate("$clingAutoload$./MoDSSAnalysis.h")))  MoDSSAnlFact;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSParameter;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSCorrelProc;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSCorrelEvent;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSCorrelFact;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelParameter.h")))  MoDSSCorrelParameter;
class __attribute__((annotate("$clingAutoload$./MoDSSDSSDEvent.h")))  MoDSSDSSDEvent;
class __attribute__((annotate("$clingAutoload$./MoDSSSingleEvent.h")))  MoDSSSingleEvent;
class __attribute__((annotate("$clingAutoload$./MoDSSCorrelFact.h")))  MoDSSHardwareParameter;
class __attribute__((annotate("$clingAutoload$./MoDSSPrintEventParameter.h")))  MoDSSPrintEventParameter;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libGo4UserAnalysis dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef Linux
  #define Linux 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "./MoDSSCorrelParameter.h"
#include "./MoDSSUnpackProc.h"
#include "./MoDSSCorrelFact.h"
#include "./MoDSSAnlProc.h"
#include "./MoDSSCorrelEvent.h"
#include "./MoDSSAnalysis.h"
#include "./MoDSSSingleEvent.h"
#include "./MoDSSUnpackFact.h"
#include "./MoDSSCorrelProc.h"
#include "./MoDSSGlobals.h"
#include "./MoDSSHardwareParameter.h"
#include "./MoDSSAnlFact.h"
#include "./MoDSSDSSDEvent.h"
#include "./MoDSSPrintEventParameter.h"
#include "./MoDSSAnlEvent.h"
#include "./MoDSSCalibParameter.h"
#include "./MoDSSParameter.h"
#include "./MoDSSFilterParameter.h"
#include "./MoDSSUnpackEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"MoDSSAnalysis", payloadCode, "@",
"MoDSSAnlEvent", payloadCode, "@",
"MoDSSAnlFact", payloadCode, "@",
"MoDSSAnlProc", payloadCode, "@",
"MoDSSCalibParameter", payloadCode, "@",
"MoDSSCorrelEvent", payloadCode, "@",
"MoDSSCorrelFact", payloadCode, "@",
"MoDSSCorrelParameter", payloadCode, "@",
"MoDSSCorrelProc", payloadCode, "@",
"MoDSSDSSDEvent", payloadCode, "@",
"MoDSSFilterParameter", payloadCode, "@",
"MoDSSHardwareParameter", payloadCode, "@",
"MoDSSParameter", payloadCode, "@",
"MoDSSPrintEventParameter", payloadCode, "@",
"MoDSSSingleEvent", payloadCode, "@",
"MoDSSUnpackEvent", payloadCode, "@",
"MoDSSUnpackFact", payloadCode, "@",
"MoDSSUnpackProc", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libGo4UserAnalysis",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libGo4UserAnalysis_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libGo4UserAnalysis_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libGo4UserAnalysis() {
  TriggerDictionaryInitialization_libGo4UserAnalysis_Impl();
}
