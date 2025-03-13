#ifndef __EARCUT_H__
#define __EARCUT_H__

#include "Type.h"
#include "GeoType.h"

#include <LogProject/Log.h>
#include <vector>

namespace EarCut
{
	namespace Internal
	{
		struct IndexNode
		{
			UINT32 index;
			IndexNode* prev;
			IndexNode* next;
	
			IndexNode()
				: index(0)
				, prev(nullptr)
				, next(nullptr)
			{}
		};

		class List
		{
			public :
				List()
					: m_pHead(nullptr)
					, m_nCount(0)
				{
					m_pHead = new IndexNode();
				}

				virtual ~List()
				{
					IndexNode* pNode = m_pHead;
					while (pNode)
					{
						IndexNode* pNext = pNode->next;
						if (pNode->prev)
						{
							pNode->prev->next = nullptr;
						}
						delete pNode;
						pNode = pNext;
					}
				}

			public :
				void Insert(const UINT32 _index)
				{
					if (Check(_index))
					{
						return;
					}

					IndexNode* pNode = new IndexNode();
					pNode->index = _index;

					if (!m_pHead)
					{
						m_pHead = new IndexNode();
						m_pHead->next = pNode;
					}
					else
					{
						IndexNode* pNext = m_pHead->next;
						IndexNode* pPrev = nullptr;
						while (pNext && (_index > pNext->index))
						{
							pPrev = pNext;
							pNext = pNext->next;
						}
	
						pNode->prev = pPrev;
						pNode->next = pNext;
	
						if (pPrev)
						{
							pPrev->next = pNode;
						}
						else
						{
							m_pHead->next = (m_pHead->next->index == pNode->index) ? pNode : m_pHead->next;
						}
	
						if (pNext)
						{
							pNext->prev = pNode;
						}
					}

					m_nCount++;
				}

				bool Check(const UINT32 _index)
				{
					bool bCheck = false;

					if (m_pHead)
					{
						IndexNode* pNode = m_pHead->next;
						const UINT32 startIndex = pNode->index;
						do
						{
							if (_index == pNode->index)
							{
								bCheck = true;
								break;
							}
							pNode = pNode->next;
						} 
						while (pNode && startIndex != pNode->index);
					}

					return bCheck;
				}
				
				void Remove(const UINT32 _index)
				{
					IndexNode* pNode = m_pHead;
					while (pNode && (_index != pNode->index))
					{
						pNode = pNode->next;
					}

					if (pNode)
					{
						IndexNode* pPrev = pNode->prev;
						IndexNode* pNext = pNode->next;

						if (pPrev)
						{
							pPrev->next = pNext;
						}

						if (pNext)
						{
							pNext->prev = pPrev;
						}

						m_pHead->next = (m_pHead->next->index == pNode->index) ? pNode->next : m_pHead->next;
						delete pNode;

						m_nCount--;
					}
				}

				void Print()
				{
					IndexNode* pNode = m_pHead;
					LOGINFO() << "[EarCut][List]";
					while (pNode)
					{
						LOGINFO() << "[EarCut][List] Index : " << pNode->index;
						pNode = pNode->next;
					}
				}

				static void Print(const IndexNode* _node)
				{
					if (_node)
					{
						const IndexNode* pPrev = _node->prev;
						const IndexNode* pNext = _node->next;
						LOGINFO() << "[EarCut][List] Prev(" << (pPrev ? pPrev->index : 0) << ")";
						LOGINFO() << "[EarCut][List] Now(" << _node->index << ")";
						LOGINFO() << "[EarCut][List] Next(" << (pNext ? pNext->index : 0) << ")";
					}
				}

				const IndexNode* Get()
				{
					IndexNode* pNode = m_pHead;
					while (pNode->next && pNode->index != m_pHead->index)
					{
						pNode = pNode->next;
					}

					pNode->prev = !pNode->prev ? m_pTail : pNode->prev;
					pNode->next = !pNode->next ? m_pHead : pNode->next;
					return pNode;
				}

				const IndexNode* Get(const UINT32 _index)
				{
					IndexNode* pNode = m_pHead;
					while (pNode && (_index != pNode->index))
					{
						pNode = pNode->next;
					}

					pNode->next = !pNode->next ? m_pHead : pNode->next;
					return pNode;
				}

				const UINT32 Count()
				{
					return m_nCount;
				}

			protected :
				IndexNode* m_pHead;

				UINT32 m_nCount;
		};
	};

	std::vector<UINT32> Composite(const std::vector<FVEC2>& _vertices = std::vector<FVEC2>())
	{
		Internal::List list;

		list.Insert(0);
		list.Insert(1);
		list.Insert(3);
		list.Insert(2);

		list.Print();

		list.Remove(0);
		list.Print();

		list.Insert(4);
		list.Remove(2);
		list.Print();

		Internal::List::Print(list.Get(1));

		return std::vector<UINT32>();
	}

	FLOAT32 Cross(const FVEC2& _v1, const FVEC2& _v2)
	{
		return _v1.x * _v2.y - _v1.y * _v2.x;
	}

	bool IsConvex(const Internal::IndexNode* _pNode, const std::vector<FVEC2>& _vertices)
	{
		if (!_pNode)
		{
			return false;
		}

		FVEC2 _now = _vertices[_pNode->index];
		FVEC2 _prev = _vertices[_pNode->prev->index];
		FVEC2 _next = _vertices[_pNode->next->index];

		FVEC2 vPrev2Now = _now - _prev;
		FVEC2 vNow2Next = _next - _now;

		return Cross(vPrev2Now, vNow2Next) >= 0.0f;
	}

	bool IsEar(const Internal::IndexNode* _pNode, const std::vector<FVEC2>& _vertices)
	{
		if (!_pNode)
		{
			return false;
		}

		FVEC2 _now = _vertices[_pNode->index];
		FVEC2 _prev = _vertices[_pNode->prev->index];
		FVEC2 _next = _vertices[_pNode->next->index];

		FVEC2 vPrev2Now = _now - _prev;
		FVEC2 vNow2Next = _next - _now;
		FVEC2 vNext2Prev = _prev - _next;
		for (UINT32 index = 0; index < static_cast<UINT32>(_vertices.size()); index++)
		{
			if (index == _pNode->index ||
				index == _pNode->prev->index ||
				index == _pNode->next->index)
			{
				continue;
			}

			FVEC2 local = _vertices[index];

			FVEC2 vPrev2Local = local - _prev;
			FVEC2 vNow2Local = local - _now;
			FVEC2 vNext2Local = local - _next;

			if (Cross(vPrev2Now, vPrev2Local) >= 0.0f &&
				Cross(vNow2Next, vNow2Local) >= 0.0f &&
				Cross(vNext2Prev, vNext2Local) >= 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<UINT32> Triangulate(const std::vector<FVEC2>& _vertices = std::vector<FVEC2>())
	{
		std::vector<UINT32> vIndex;

		if (_vertices.size() < 3)
		{
			return vIndex;
		}

		Internal::List indexList;
		Internal::List reflexList;
		Internal::List convexList;
		Internal::List earList;

		for (UINT32 index = 0; index < static_cast<UINT32>(_vertices.size()); index++)
		{
			indexList.Insert(index);
		}

		for (UINT32 index = 0; index < static_cast<UINT32>(_vertices.size()); index++)
		{
			if (const Internal::IndexNode* pNode = indexList.Get(index))
			{
				if (IsConvex(pNode, _vertices))
				{
					convexList.Insert(pNode->index);

					if (IsEar(pNode, _vertices))
					{
						earList.Insert(pNode->index);
					}
				}
				else
				{
					reflexList.Insert(pNode->index);
				}
			}
		}

		while (indexList.Count() > 3)
		{
			const Internal::IndexNode* pEar = indexList.Get(earList.Get()->index);
			if (pEar)
			{
				UINT32 earIndex = pEar->index;
				UINT32 prevIndex = pEar->prev->index;
				UINT32 nextIndex = pEar->next->index;

				vIndex.push_back(prevIndex);
				vIndex.push_back(earIndex);
				vIndex.push_back(nextIndex);

				indexList.Remove(earIndex);
				convexList.Remove(earIndex);
				reflexList.Remove(earIndex);
				earList.Remove(earIndex);

				if (const Internal::IndexNode* pPrev = indexList.Get(prevIndex))
				{
					if (IsConvex(pPrev, _vertices))
					{
						convexList.Insert(pPrev->index);

						if (IsEar(pPrev, _vertices))
						{
							earList.Insert(pPrev->index);
						}
					}
					else
					{
						reflexList.Insert(pPrev->index);
					}
				}

				if (const Internal::IndexNode* pNext = indexList.Get(nextIndex))
				{
					if (IsConvex(pNext, _vertices))
					{
						convexList.Insert(pNext->index);

						if (IsEar(pNext, _vertices))
						{
							earList.Insert(pNext->index);
						}
					}
					else
					{
						reflexList.Insert(pNext->index);
					}
				}
			}
		}

		
		return vIndex;
	}
};

#endif // __EARCUT_H__