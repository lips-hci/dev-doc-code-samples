import java.util.List;
import org.openni.DeviceInfo;
import org.openni.OpenNI;

class NiHello{

	public static void main(String[] args){
		OpenNI.initialize();
		
		List<DeviceInfo> devices = OpenNI.enumerateDevices();
		
		for (DeviceInfo d : devices) {
	    		System.out.println("Name:" + d.getName());
	    		System.out.println("Uri:" + d.getUri());
	    		System.out.println("USB Product ID:" + d.getUsbProductId());
		    	System.out.println("USB Vendor ID:" + d.getUsbVendorId());
	    		System.out.println("Vendor:" + d.getVendor());
		}

		Device device = Device.open();
		
		device.close();
		OpenNI.shutdown();
	}
}
