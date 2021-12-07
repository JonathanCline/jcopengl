-- Grab libs
local iter = require("iter")
local algorithm = require("algorithm")

-- CLI args
local args = { ... }

-- Print usage if no arguements were given
if #args == 0 then
    print("missing define dump file path!")
    return 0
end


-- Reads the defines file in from disk and returns
-- a table index by opengl version
local function read_defines_file(_pathToDefines)
    
    -- Read file lines
    local _lines = {}
    do
        local _file = io.open(args[1], "r")
        for l in _file:lines() do
            _lines[#_lines+1] = l
        end
        io.close(_file)
    end

    local _versions = {}
    local _current

    for i, v in ipairs(_lines) do
        if string.find(v, "^#define ") then
            local _str = string.sub(v, 9)
            if string.find(_str, "^GL_VERSION") then

                local _vm, _vn = string.match(_str, "GL_VERSION_(%d)_(%d)")

                local _vmi = tonumber(_vm)
                local _vni = tonumber(_vn)

                local _verMajorTable = _versions[_vmi]
                if not _verMajorTable then
                    _versions[_vmi] = {}
                    _verMajorTable = _versions[_vmi]
                end
                _versions[_vmi][_vni] = {}

                _current = _versions[_vmi][_vni]

            elseif _current then
                _current[#_current+1] = _str
            end
        end
    end

    return _versions
end

local _versions = read_defines_file(args[1])
algorithm.foreach(_versions[4][5], function (_, v) print(v) end)


