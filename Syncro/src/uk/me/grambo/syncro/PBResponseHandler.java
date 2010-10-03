package uk.me.grambo.syncro;

import java.io.InputStream;

public interface PBResponseHandler {
	public boolean canHandleResponse(int innType);
	public boolean handleResponse(int nSubpacketSizes[],InputStream inoStream);
	public boolean canRemove();
}
