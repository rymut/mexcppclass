/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/msgtext.hpp"

#include <string>
#include <map>

#include "mexcppclass/matlab/msgid.hpp"
#include "mexcppclass/utility/map.hpp"

namespace mexcppclass {
namespace matlab {

std::string MsgTextName(const MsgId &msg_id) {
  static const std::map<MsgId, std::string> msg_id_name_map = {
    { MsgId::MemoryAllocationFailure, "memoryAllocationFailure" },
    { MsgId::NotEnoughInputs, "notEnoughInputs" },
    { MsgId::ToManyInputs, "toManyInputs" },

    { MsgId::NotEnoughOutputs, "notEnoughOutputs" },
    { MsgId::ToManyOutputs, "toManyOutputs" },

    { MsgId::NotEnoughDimensions, "notEnoughDimensions" },
    { MsgId::ToManyDimensions, "toManyDimensions" },

    { MsgId::DimensionLenghtToSmall, "dimensionLenghtToSmall" },
    { MsgId::DimensionLenghtToBig, "demensionLenghtToBig" },

    { MsgId::InvalidInput, "invalidInput" },
    { MsgId::InvalidOutput, "invalidOutput" },
    { MsgId::InvalidArguments, "invalidArguments" },
    { MsgId::InvalidArgument, "invalidArgument" },


    { MsgId::InvalidCommand, "invalidCommand" },
    { MsgId::InvalidHandle, "invalidHandle" },
    { MsgId::InvalidType, "invalidType" },
    { MsgId::InvalidMethod, "invalidMethod" },
    { MsgId::InvalidClassId, "invalidClassId"},

    { MsgId::RequiredArgumentMissing, "requiredArgumentMissing"},
    { MsgId::Custom, "custom" },
    { MsgId::NotImplemented, "notImplemented" },
    { MsgId::None, "none" }
  };

  return utility::MapValue(msg_id_name_map, msg_id).value_or("undefined");
}

std::string MsgTextDesc(const MsgId &msg_id) {
  static const std::map<MsgId, std::string> msg_id_desc_map = {
    { MsgId::MemoryAllocationFailure, "Memory allocation failure" },
    { MsgId::NotEnoughInputs,  "Not enought input arguments" },
    { MsgId::ToManyInputs, "To many input arguments" },

    { MsgId::NotEnoughOutputs, "Not enough output arguments" },
    { MsgId::ToManyOutputs, "To many output arguments" },

    { MsgId::NotEnoughDimensions, "Not enough dimensions" },
    { MsgId::ToManyDimensions,  "To many dimensions" },

    { MsgId::DimensionLenghtToSmall, "Dimension lenght is to small" },
    { MsgId::DimensionLenghtToBig, "Dimension lenght is to big" },

    { MsgId::InvalidInput, "Invalid input arguments" },
    { MsgId::InvalidOutput, "Invalid output arguments" },
    { MsgId::InvalidArguments, "Invalid arguments" },
    { MsgId::InvalidArgument, "Invalid argument" },

    { MsgId::InvalidCommand, "Invalid command" },
    { MsgId::InvalidHandle, "Invalid handle" },
    { MsgId::InvalidType, "Invalid type" },
    { MsgId::InvalidMethod, "Invalid method" },
    { MsgId::InvalidClassId, "Invalid ClassID" },

    { MsgId::RequiredArgumentMissing, "Missing required argument" },
    { MsgId::Custom, "Custom" },
    { MsgId::NotImplemented, "Not implemented" },
    { MsgId::None, "" }
  };

  return utility::MapValue(msg_id_desc_map, msg_id).value_or("undefined");
}

}  // namespace matlab
}  // namespace mexcppclass
