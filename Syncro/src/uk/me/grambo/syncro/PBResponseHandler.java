package uk.me.grambo.syncro;

import java.io.IOException;
import java.io.InputStream;

public interface PBResponseHandler {
	public boolean canHandleResponse(int innType);
	public boolean handleResponse(int nSubpacketSizes[],InputStream inoStream) throws Exception,IOException;
	public boolean canRemove();
}
