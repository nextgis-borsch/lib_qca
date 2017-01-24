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

#pragma once

#include "ringwatch.h"
#include "gpgop.h"
#include "qcaprovider.h"
#include "mykeystoreentry.h"
#include <QMutex>

namespace gpgQCAPlugin
{

class MyKeyStoreList : public QCA::KeyStoreListContext
{
	Q_OBJECT
public:
	int init_step;
	bool initialized;
	GpgOp gpg;
	GpgOp::KeyList pubkeys, seckeys;
	QString pubring, secring, homeDir;
	bool pubdirty, secdirty;
	RingWatch ringWatch;
	QMutex ringMutex;

	MyKeyStoreList(QCA::Provider *p);
	~MyKeyStoreList();

	// reimplemented Provider::Context
	QCA::Provider::Context *clone() const;

	// reimplemented KeyStoreListContext
	QString name(int) const;
	QCA::KeyStore::Type type(int) const;
	QString storeId(int) const;
	QList<int> keyStores();

	void start();

	bool isReadOnly(int) const;

	QList<QCA::KeyStoreEntry::Type> entryTypes(int) const;
	QList<QCA::KeyStoreEntryContext*> entryList(int);
	QCA::KeyStoreEntryContext *entry(int, const QString &entryId);
	QCA::KeyStoreEntryContext *entryPassive(const QString &serialized);
	QString writeEntry(int, const QCA::PGPKey &key);
	bool removeEntry(int, const QString &entryId);

	static MyKeyStoreList *instance();
	void ext_keyStoreLog(const QString &str);

	QCA::PGPKey getPubKey(const QString &keyId) const;
	QCA::PGPKey getSecKey(const QString &keyId, const QStringList &userIdsOverride) const;
	QCA::PGPKey publicKeyFromId(const QString &keyId);
	QCA::PGPKey secretKeyFromId(const QString &keyId);

private slots:
	void gpg_finished();
	void ring_changed(const QString &filePath);

private:
	void pub_changed();
	void sec_changed();
	void handleDirtyRings();
};

} // end namespace gpgQCAPlugin
