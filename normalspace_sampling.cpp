#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>


/**
 * @description:			���߲���
 * @param cloud				�������
 * @param cloud_filtered	�˲�����
 * @param sample_ratio		��������
 */
void normalspacesample(pcl::PointCloud<pcl::PointXYZ>::Ptr& cloud, pcl::PointCloud<pcl::PointXYZ>::Ptr& cloud_filtered, int k, float sample_ratio)
{
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);   //�������������������KD-Tree
	pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);		//�洢�������
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normalEstimation;						//�������߹�������
	normalEstimation.setInputCloud(cloud);													//�������
	normalEstimation.setKSearch(k);															//ʹ�õ�ǰ����Χ����ĸ���
	normalEstimation.compute(*cloud_normals);												//���㷨��

	//ͨ��concatenateFields������point��normal��������γ�PointNormal��������
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normal(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*cloud, *cloud_normals, *cloud_with_normal);

	std::sort(cloud_with_normal->begin(), cloud_with_normal->end(), [](pcl::PointNormal pt1, pcl::PointNormal pt2) {return pt1.curvature>pt2.curvature; });
	for (size_t i = 0; i < sample_ratio*cloud->size(); i++)
	{
		cloud_filtered->push_back(pcl::PointXYZ(cloud_with_normal->points[i].x, cloud_with_normal->points[i].y, cloud_with_normal->points[i].z));
	}
}


int main(int argc, char* argv[])
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::io::loadPCDFile("rabbit.pcd", *cloud);

	normalspacesample(cloud, cloud_filtered, 20, 0.1f);
	pcl::io::savePCDFile("mynormalspacesample.pcd", *cloud_filtered);

	return EXIT_SUCCESS;
}