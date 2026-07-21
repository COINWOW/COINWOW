// Copyright (C) 2009-2025 Bitcoin Core developers

// Copyright (C) 2026 COINWOW developers

// Distributed under the MIT software license

// Copyright (c) 2021 The COINWOW Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef EXAMPLE_PRINTER_H
#define EXAMPLE_PRINTER_H

#include <string>

class Printer
{
public:
    virtual ~Printer() = default;
    virtual void print(const std::string& message) = 0;
};

#endif // EXAMPLE_PRINTER_H
