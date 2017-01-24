/*
 * Copyright (C) 2003-2008  Justin Karneges <justin@affinix.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "myopenpgpcontext.h"
#include "mymessagecontext.h"

using namespace QCA;

namespace gpgQCAPlugin
{

MyOpenPGPContext::MyOpenPGPContext(QCA::Provider *p)
	: SMSContext(p, "openpgp")
{
	// TODO
}

Provider::Context *MyOpenPGPContext::clone() const
{
	return 0;
}

MessageContext *MyOpenPGPContext::createMessage()
{
	return new MyMessageContext(this, provider());
}

} // end namespace gpgQCAPlugin
