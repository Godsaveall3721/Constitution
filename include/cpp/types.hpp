#ifndef CONSTITUTION_CPP_TYPES_HPP
#define CONSTITUTION_CPP_TYPES_HPP

extern "C" {
#include "types.h"
}

namespace republic {
namespace cpp {
using Status = constitution_status_t;
using BodyKind = constitution_body_kind_t;
using OfficeKind = constitution_office_kind_t;
using ProcedureKind = constitution_procedure_kind_t;
using DocumentKind = constitution_document_kind_t;
using ElectionKind = constitution_election_kind_t;
}
}

#endif
