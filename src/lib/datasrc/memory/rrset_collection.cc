// Copyright (C) 2013  Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <datasrc/memory/rrset_collection.h>
#include <datasrc/memory/treenode_rrset.h>

#include <exceptions/exceptions.h>

using namespace bundy;
using namespace bundy::dns;

namespace bundy {
namespace datasrc {
namespace memory {

ConstRRsetPtr
RRsetCollection::find(const bundy::dns::Name& name,
                      const bundy::dns::RRClass& rrclass,
                      const bundy::dns::RRType& rrtype) const
{
    if (rrclass != rrclass_) {
        // We could throw an exception here, but RRsetCollection is
        // expected to support an arbitrary collection of RRsets, and it
        // can be queried just as arbitrarily. So we just return nothing
        // here.
        return (ConstRRsetPtr());
    }

    const ZoneTree& tree = zone_data_.getZoneTree();
    const ZoneNode *node = NULL;
    ZoneTree::Result result = tree.find(name, &node);
    if (result != ZoneTree::EXACTMATCH) {
        return (ConstRRsetPtr());
    }

    const RdataSet* rdataset = RdataSet::find(node->getData(), rrtype);
    if (rdataset == NULL) {
        return (ConstRRsetPtr());
    }

    return (ConstRRsetPtr(new TreeNodeRRset(rrclass_, node, rdataset, true)));
}

} // end of namespace memory
} // end of namespace datasrc
} // end of namespace bundy
