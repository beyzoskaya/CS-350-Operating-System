import numpy as np
import matplotlib.pyplot as plt

# Load the data
file_sizes = np.loadtxt('file_sizes.txt')

if file_sizes.size == 0:
    raise ValueError("No data found in file_sizes.txt.")

print("File Sizes Loaded:", file_sizes[:10]) 

bins = np.logspace(np.log10(1), np.log10(file_sizes.max()), 100)
pdf, bins = np.histogram(file_sizes, bins=bins, density=True)
print("PDF:", pdf[:10]) 
print("Bins:", bins[:10]) 

cdf = np.cumsum(pdf * np.diff(bins))
print("CDF:", cdf[:10]) 

plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(bins[:-1], pdf, label='PDF')
plt.plot(bins[:-1], cdf, label='CDF')
plt.xlabel('File Size')
plt.ylabel('Cumulative Probability')
plt.legend()
plt.title('PDF and CDF on Linear Scale')
plt.grid(True)

plt.savefig('pdf_cdf_linear.png')
print("Linear scale plot saved as pdf_cdf_linear.png")

plt.subplot(1, 2, 2)
plt.plot(bins[:-1], pdf, label='PDF')
plt.plot(bins[:-1], cdf, label='CDF')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('File Size (log scale)')
plt.ylabel('Cumulative Probability (log scale)')
plt.legend()
plt.title('PDF and CDF on Logarithmic Scale')
plt.grid(True)

plt.savefig('pdf_cdf_logarithmic.png')
print("Logarithmic scale plot saved as pdf_cdf_logarithmic.png")

plt.tight_layout()
plt.show()